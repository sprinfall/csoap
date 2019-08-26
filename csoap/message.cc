#include "csoap/message.h"

#include <cassert>

#include "csoap/xml.h"

namespace csoap {

void Message::ToXml(bool format_raw, const std::string& indent,
                    std::string* xml_string) {
  assert(soapenv_ns_.IsValid() && service_ns_.IsValid() && !operation_.empty());

  pugi::xml_document xdoc;

  pugi::xml_node xroot = xml::AddChild(xdoc, soapenv_ns_.name, "Envelope");

  xml::AddNSAttr(xroot, soapenv_ns_.name, soapenv_ns_.url);

  pugi::xml_node xbody = xml::AddChild(xroot, soapenv_ns_.name, "Body");

  ToXmlBody(xbody);

  xml::XmlStringWriter writer(xml_string);

  unsigned int flags = format_raw ? pugi::format_raw : pugi::format_indent;

  // Use print() instead of save() for no declaration or BOM.
  xdoc.print(writer, indent.c_str(), flags, pugi::encoding_utf8);
}

bool Message::FromXml(const std::string& xml_string) {
  pugi::xml_document xdoc;
  pugi::xml_parse_result result = xdoc.load_string(xml_string.c_str());

  if (!result) {
    return false;
  }

  pugi::xml_node xroot = xdoc.document_element();

  soapenv_ns_.name = xml::GetPrefix(xroot);
  soapenv_ns_.url = xml::GetNSAttr(xroot, soapenv_ns_.name);

  pugi::xml_node xbody = xml::GetChild(xroot, soapenv_ns_.name, "Body");
  if (xbody) {
    return FromXmlBody(xbody);
  }

  return false;
}

}  // namespace csoap
