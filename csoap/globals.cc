#include "csoap/globals.h"

#include <ostream>

namespace csoap {

const SoapNamespace kSoapEnvNamespaceV11{
  "soap",
  "http://schemas.xmlsoap.org/soap/envelope/"
};

const SoapNamespace kSoapEnvNamespaceV12{
  "soap",
  "http://www.w3.org/2003/05/soap-envelope"
};

std::ostream& operator<<(std::ostream& os, const SoapFault& fault) {
  os << "Fault: {" << std::endl
     << "  faultcode: " << fault.faultcode << std::endl
     << "  faultstring: " << fault.faultstring << std::endl
     << "  detail: " << fault.detail << std::endl
     << "}";
  return os;
}

}  // namespace csoap
