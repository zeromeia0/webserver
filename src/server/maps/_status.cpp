#include "../../main.hpp"

struct _status {
	int			code;
	std::string	msg;
};

_status m_status[] = {
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

std::string getStatusMsg(int code) {
	for (size_t i = 0; i < (sizeof(m_status) / sizeof(m_status[0])); i++) {
		if (m_status[i].code == code)
			return (m_status[i].msg);
	}
	return ("Unknown");
}
