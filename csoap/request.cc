#include "csoap/request.h"

#include "csoap/xml.h"

namespace csoap {

void Request::AddParameter(const Parameter& parameter) {
  parameters_.push_back(parameter);
}

void Request::AddParameter(Parameter&& parameter) {
  parameters_.push_back(std::move(parameter));
}

const std::string& Request::GetParameter(const std::string& key) const {
  for (const Parameter& p : parameters_) {
    if (p.key() == key) {
      return p.value();
    }
  }

  static const std::string kEmptyValue;
  return kEmptyValue;
}

void Request::ToXmlBody(pugi::xml_node xbody) {
  pugi::xml_node xop = xml::AddChild(xbody, service_ns_.name, operation_);
  xml::AddNSAttr(xop, service_ns_.name, service_ns_.url);

  for (Parameter& p : parameters_) {
    pugi::xml_node xparam = xml::AddChild(xop, service_ns_.name, p.key());

    // xparam.text().set() also works for PCDATA.
    xparam.append_child(p.as_cdata() ? pugi::node_cdata : pugi::node_pcdata)
        .set_value(p.c_value());
  }
}

bool Request::FromXmlBody(pugi::xml_node xbody) {
  pugi::xml_node xoperation = xbody.first_child();
  if (!xoperation) {
    return false;
  }

  xml::SplitName(xoperation, &service_ns_.name, &operation_);
  service_ns_.url = xml::GetNSAttr(xoperation, service_ns_.name);

  pugi::xml_node xparameter = xoperation.first_child();
  while (xparameter) {
    parameters_.push_back({
      xml::GetNameNoPrefix(xparameter),
      // xparameter.text().get/as_string() also works.
      std::string(xparameter.child_value())
    });

    xparameter = xparameter.next_sibling();
  }

  return true;
}

}  // namespace csoap
