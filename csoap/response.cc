#include "csoap/response.h"

#include <cassert>

#include "csoap/xml.h"

namespace csoap {

void Response::ToXmlBody(pugi::xml_node xbody) {
  pugi::xml_node xresponse = xml::AddChild(xbody, service_ns_.name,
                                           operation_ + "Response");
  xml::AddNSAttr(xresponse, service_ns_.name, service_ns_.url);

  if (simple_result_) {
    pugi::xml_node xresult = xml::AddChild(xresponse, service_ns_.name,
                                           simple_result_->name);
    xml::SetText(xresult, simple_result_->value, simple_result_->is_cdata);
  } else {
    assert(composer_);
    (*composer_)(xresponse);
  }
}

bool Response::FromXmlBody(pugi::xml_node xbody) {
  assert(parser_);

  // Check Fault element.

  pugi::xml_node xfault = xml::GetChildNoNS(xbody, "Fault");

  // TODO: service_ns_.url

  if (xfault) {
    fault_.reset(new SoapFault);

    pugi::xml_node xfaultcode = xml::GetChildNoNS(xfault, "faultcode");
    pugi::xml_node xfaultstring = xml::GetChildNoNS(xfault, "faultstring");
    pugi::xml_node xdetail = xml::GetChildNoNS(xfault, "detail");

    if (xfaultcode) {
      fault_->faultcode = xfaultcode.text().as_string();
    }
    if (xfaultstring) {
      fault_->faultstring = xfaultstring.text().as_string();
    }
    if (xdetail) {
      fault_->detail = xdetail.text().as_string();
    }

    return false;
  }

  // Check Response element.

  pugi::xml_node xresponse = xml::GetChildNoNS(xbody, operation_ + "Response");
  if (!xresponse) {
    return false;
  }

  xml::SplitName(xresponse, &service_ns_.name, nullptr);
  service_ns_.url = xml::GetNSAttr(xresponse, service_ns_.name);

  // Call result parser on each child of the response node.
  pugi::xml_node xchild = xresponse.first_child();
  while (xchild) {
    if (!parser_(xchild)) {
      break;
    }
    xchild = xchild.next_sibling();
  }

  return true;
}

}  // namespace csoap
