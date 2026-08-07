#include "map.hpp"

#define DEFAULT_MIME "application/octet-stream"

reMime MAP_mime[] = {
	{ ".html", "text/html" },
	{ ".css",  "text/css" },
	{ ".js",   "application/javascript" },
	{ ".webp", "image/webp" },
	{ ".jpg",  "image/jpeg" },
	{ ".jpeg", "image/jpeg" },
	{ ".png",  "image/png" },
	{ ".gif",  "image/gif"},
	{ ".ico",  "image/x-icon" },
	{ ".txt",  "text/plain"},
	{ ".json", "application/json" },
	{ ".pdf",  "application/pdf" },
};

reStatusCode MAP_status_code[] = {
	{ 200, "OK" },
	{ 201, "Created" },
	{ 301, "Moved Permanently" },
	{ 403, "Forbidden" },
	{ 404, "Not Found" },
	{ 405, "Method Not Allowed" },
	{ 413, "Payload Too Large" },
	{ 500, "Internal Server Error" },
	{ 504, "Gateway Timeout" },
};

method MAP_method[] = {
	{ GET, "GET" },
	{ POST, "POST" },
	{ DELETE, "DELETE" },
	{ HEAD, "HEAD" },
};

std::string *getMimeType(std::string extension) {
	for (size_t i = 0; i < (sizeof(MAP_mime) / sizeof(MAP_mime[0])); i++) {
		if (MAP_mime[i].extension == extension)
			return (&MAP_mime[i].mime);
	}
	return (NULL);
}

std::string *getMimeExtension(std::string mime) {
	for (size_t i = 0; i < (sizeof(MAP_mime) / sizeof(MAP_mime[0])); i++) {
		if (MAP_mime[i].mime == mime)
			return (&MAP_mime[i].extension);
	}
	return (NULL);
}

std::string *getStatusMsg(int status_code) {
	for (size_t i = 0; i < (sizeof(MAP_status_code) / sizeof(MAP_status_code[0])); i++) {
		if (MAP_status_code[i].code == status_code)
			return (&MAP_status_code[i].msg);
	}
	return (NULL);
}

std::string *getMethodTxt(eMethod code) {
	for (size_t i = 0; i < (sizeof(MAP_method) / sizeof(MAP_method[0])); i++) {
		if (MAP_method[i].code == code)
			return (&MAP_method[i].txt);
	}
	return (NULL);
}

eMethod *getMethodCode(std::string txt) {
	for (size_t i = 0; i < (sizeof(MAP_method) / sizeof(MAP_method[0])); i++) {
		if (MAP_method[i].txt == txt)
			return (&MAP_method[i].code);
	}
	return (NULL);
}
