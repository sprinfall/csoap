#include "csoap/client.h"

#include <cassert>
#include <utility>

#include "boost/algorithm/string.hpp"

#include "webcc/client_session.h"

#include "csoap/request.h"

namespace csoap {

Client::Client(const std::string& url, SoapVersion soap_version)
    : url_(url), soap_version_(soap_version), format_raw_(true),
      session_(new webcc::ClientSession{}),
      http_status_(webcc::Status::kOK) {
}

Client::~Client() {
  delete session_;
}

void Client::SetTimeout(int seconds) {
  session_->set_timeout(seconds);
}

bool Client::Request(const std::string& operation,
                     std::vector<Parameter>&& parameters,
                     Response::Parser parser,
                     std::size_t buffer_size) {
  assert(service_ns_.IsValid());
  assert(!url_.empty());

  error_.Set(webcc::Error::kOK, "OK");
  fault_.reset();

  csoap::Request request;

  // Set SOAP envelope namespace according to SOAP version.
  if (soap_version_ == kSoapV11) {
    request.set_soapenv_ns(kSoapEnvNamespaceV11);
  } else {
    request.set_soapenv_ns(kSoapEnvNamespaceV12);
  }

  request.set_service_ns(service_ns_);

  request.set_operation(operation);

  for (Parameter& p : parameters) {
    request.AddParameter(std::move(p));
  }

  std::string http_content;
  request.ToXml(format_raw_, indent_str_, &http_content);

  try {
    webcc::RequestBuilder builder;
    builder.Post(url_);
    builder.Body(std::move(http_content));

    auto http_request = builder();

    // NOTE:
    // According to www.w3.org when placing SOAP messages in HTTP bodies, the HTTP
    // Content-type header must be chosen as "application/soap+xml" [RFC 3902].
    // But in practice, many web servers cannot understand it.
    // See: https://www.w3.org/TR/2007/REC-soap12-part0-20070427/#L26854
    if (soap_version_ == kSoapV11) {
      http_request->SetContentType(webcc::media_types::kTextXml,
                                   webcc::charsets::kUtf8);
    } else {
      http_request->SetContentType(webcc::media_types::kApplicationSoapXml,
                                   webcc::charsets::kUtf8);
    }

    http_request->SetHeader("SOAPAction", operation);

    session_->set_buffer_size(buffer_size);

    auto http_response = session_->Send(http_request);

    // Save the HTTP status code.
    http_status_ = http_response->status();

    Response response;
    response.set_operation(operation);
    response.set_parser(parser);

    if (!response.FromXml(http_response->data())) {
      if (response.fault()) {
        fault_ = response.fault();
        error_.Set(webcc::Error::kUnknownError, "SOAP fault");
      } else {
        error_.Set(webcc::Error::kParseError, "XML parsing error");
      }
      return false;
    }

    return true;

  } catch (const webcc::Error& e) {
    error_ = e;
    return false;
  }
}

bool Client::Request(const std::string& operation,
                     std::vector<Parameter>&& parameters,
                     const std::string& result_name,
                     std::size_t buffer_size,
                     std::string* result) {
  auto parser = [result, &result_name](pugi::xml_node xnode) {
    if (boost::iequals(xml::GetNameNoPrefix(xnode), result_name)) {
      xml::GetText(xnode, result);
    }
    return false;  // Stop next call.
  };

  return Request(operation, std::move(parameters), parser, buffer_size);
}

}  // namespace csoap
