#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#endif

#include "tcp.h"
#include "binary_stream.h"
#include "byteswap.h"
#include "singletone.h"

namespace mtproto::transport {

	tcp_intermediate::tcp_intermediate(const char* address, uint16_t port) {
		m_address = net::ip::tcp::endpoint(net::ip::address::from_string(address), port);
	}

	tcp_intermediate::tcp_intermediate(tcp_intermediate&& rhs) noexcept : tcp(std::move(rhs)) {
		m_address = std::move(rhs.m_address);
	}

	tcp_intermediate::tcp_intermediate(uint32_t ip, uint16_t port) {
		m_address = net::ip::tcp::endpoint(net::ip::address_v4(ip), port);
	}

	tcp::tcp() : m_socket(global_singletone<net::io_context>::instance()) {}
	
	tcp::tcp(tcp&& rhs) noexcept : m_socket(std::move(rhs.m_socket)), m_is_connected(rhs.m_is_connected) {}

	bool tcp::is_connected() {
		return m_is_connected;
	}

	void tcp::swap(uint32_t& value) {
		if (!mtproto::data::binary_stream::is_little_endian) {
			value = _byteswap_ulong(value);
		}
	}

	void tcp::send_bytes(const char* data, int length) {
		error_t error;

		m_socket.send(net::buffer(data, length), 0, error);

		if (error) {
			throw mtproto::transport::invalid_socket();
		}
	}

	int tcp::receive_bytes(char* buffer, int length) {
		error_t error;

		size_t result = m_socket.receive(net::buffer(buffer, length), 0, error);

		if (error) {
			throw mtproto::transport::invalid_socket();
		}

		return static_cast<int>(result);
	}

	void tcp_intermediate::connect() {
		m_socket.connect(m_address);
		char connection_preface[4];
		memset(connection_preface, 0xee, 4);
		send_bytes(connection_preface, 4);
	}

	void tcp_intermediate::send(mtproto::data::binary_buffer&& buffer) {
		mtproto::data::binary_buffer send_buffer = std::move(buffer);
		uint32_t length = send_buffer.get_size();
		swap(length);
		send_buffer.insert_begin((char*)&length, 4);
		send_bytes(send_buffer.get_data_pointer(), send_buffer.get_size());
	}

	mtproto::data::binary_buffer tcp_intermediate::receive() {
		uint32_t length;
		if (receive_bytes((char*)&length, 4) < 4) {
			return mtproto::data::binary_buffer();
		}
		swap(length);
		mtproto::data::binary_buffer buffer;
		buffer.set_size(length);
		receive_bytes((char*)buffer.get_data_pointer(), length);
		return buffer;
	}

	tcp::~tcp() {
	}

	void tcp::close() {
		m_socket.close();
	}
}