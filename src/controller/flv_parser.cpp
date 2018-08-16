// Copyright (c) 2018 winking324
//


#include "controller/flv_parser.h"

#include <QDebug>


namespace flv_parser {


FlvParser::FlvParser(QObject *parent)
  : QObject(parent) {
  Reset();
}

FlvParser::~FlvParser() {
}

bool FlvParser::ParseData(const std::string &data) {
  buffer_ += data;

  if (!has_header_) {
    size_t header_size = ParseHeader();
    if (header_size > 0) {
      buffer_.erase(0, header_size);
    } else {
      return false;
    }
  }

  size_t body_size = 0;
  while ((body_size = ParseBody()) > 0) {
    buffer_.erase(0, body_size);
  }
  return true;
}

void FlvParser::Reset() {
  buffer_.clear();
  has_header_ = false;
}

size_t FlvParser::ParseHeader() {
  size_t read_size = flv_header_.ParseData(buffer_);
  if (read_size > 0) {
    has_header_ = true;
  }
  return read_size;
}

size_t FlvParser::ParseBody() {
  FlvBody body;
  size_t read_size = body.ParseData(buffer_);
  if (read_size > 0) {
    flv_bodys_.push_back(body);
  }
  return read_size;
}


}  // namespace flv_parser

