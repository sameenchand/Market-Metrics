#pragma once

#include "AVLTree.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/asyncrt_utils.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::json;

std::string GET_API_KEY(const std::string& filePath);
void importData(AVLTree& stockTree, std::string ticker);