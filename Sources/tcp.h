#pragma once

#include "transport.h"
#include <boost/asio.hpp>

namespace net = boost::asio;

namespace mtproto::transport {

	typedef net::ip::tcp::socket socket_t;
	typedef net::ip::tcp::endpoint address_t;
	typedef boost::system::error_code error_t;

	class invalid_socket : public std::exception {
	public:
		virtual const char* what() const noexcept override {
			return "invalid connection!";
		}
	};

	class tcp : public transport {
	protected:
		socket_t m_socket;
		address_t m_address;
		bool m_is_connected = false;
		void send_bytes(const char* data, int length);
		int receive_bytes(char* buffer, int length);
		void swap(uint32_t& value);
	public:
		tcp();
		tcp(tcp&& rhs) noexcept;
		virtual void connect() = 0;
		virtual void close();
		virtual void send(mtproto::data::binary_buffer&& buffer) = 0;
		virtual bool is_connected();
		virtual mtproto::data::binary_buffer receive() = 0;
		~tcp();
	};

	class tcp_intermediate : public tcp {
	public:
		tcp_intermediate() = delete;
		tcp_intermediate(const char* address, uint16_t port);
		tcp_intermediate(uint32_t ip, uint16_t port);
		tcp_intermediate(tcp_intermediate&& rhs) noexcept;
		virtual void connect();
		virtual void send(mtproto::data::binary_buffer&& buffer);
		virtual mtproto::data::binary_buffer receive();
	};
}
