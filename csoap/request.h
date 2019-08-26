#ifndef CSOAP_REQUEST_H_
#define CSOAP_REQUEST_H_

#include <string>
#include <vector>

#include "csoap/message.h"
#include "csoap/parameter.h"

namespace csoap {

// SOAP request.
// Used to compose the SOAP request envelope XML which will be sent as the HTTP
// request body.
class Request : public Message {
public:
  void AddParameter(const Parameter& parameter);

  void AddParameter(Parameter&& parameter);

  // Get parameter value by key.
  const std::string& GetParameter(const std::string& key) const;

protected:
  void ToXmlBody(pugi::xml_node xbody) final;
  bool FromXmlBody(pugi::xml_node xbody) final;

private:
  std::vector<Parameter> parameters_;
};

}  // namespace csoap

#endif  // CSOAP_REQUEST_H_
