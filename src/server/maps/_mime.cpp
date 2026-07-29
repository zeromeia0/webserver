#include "../../main.hpp"

struct _mime {
	std::string	extension;
	std::string	mime;
};

_mime m_mime[] = {
	{ "html", "text/html" },
	{ "css",  "text/css" },
	{ "js",   "application/javascript" },
	{ "webp", "image/webp" },
	{ "jpg",  "image/jpeg" },
	{ "jpeg", "image/jpeg" },
	{ "png",  "image/png" },
	{ "gif",  "image/gif"},
	{ "ico",  "image/x-icon" },
	{ "txt",  "text/plain"},
	{ "json", "application/json" },
	{ "pdf",  "application/pdf" },
};

std::string defaultMime = "application/octet-stream";

std::string *getMime(std::string extension) {
	for (size_t i = 0; i < (sizeof(m_mime) / sizeof(m_mime[0])); i++) {
		if (m_mime[i].extension == extension)
			return (&m_mime[i].mime);
	}
	return (&defaultMime);
}

std::string *getExtension(std::string mime) {
	for (size_t i = 0; i < (sizeof(m_mime) / sizeof(m_mime[0])); i++) {
		if (m_mime[i].mime == mime)
			return (&m_mime[i].extension);
	}
	return (NULL);
}
