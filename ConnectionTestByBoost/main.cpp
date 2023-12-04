// Booast 라이브러리(Beast, Asio)를 이용한 HTTP 통신 테스트 : HTTP GET 요청 후 응답 출력
// Booast 1.83 사용 (https://www.boost.org/)

#include <iostream>
#include <string>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

using namespace std;


int main(int argc, char* argv[])
{
	boost::asio::io_context ioc;
	boost::asio::ip::tcp::resolver resolver(ioc);
	boost::beast::tcp_stream stream(ioc);

	try
	{

		// 샘플
		// auto const host = "jsonplaceholder.typicode.com";
		// auto const port = "80";
		// auto const target = "/posts/1";

		// S_OK
		// http://192.168.0.39:8080/ebt_webapp/rest/ppr/i/login/ispark/dev01/dev01.json
		// S_OK
		// http://192.168.0.39:8080/ebt_webapp/rest/ppr/i/nextoid/relOper.json
		auto const host = "192.168.0.39";
		auto const port = "8080";
		auto const target = "/ebt_webapp/rest/ppr/i/login/ispark/dev01/dev01.json";

		bool isVer1_0 = false;
		int version = isVer1_0 ? 10 : 11;

		auto const results = resolver.resolve(host, port);
		stream.connect(results);


		string urlHost = host;
		urlHost += ":";
		urlHost += port;

		boost::beast::http::request<boost::beast::http::string_body> req{ boost::beast::http::verb::get, target, version };
		req.set(boost::beast::http::field::host, urlHost);
		req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		boost::beast::http::write(stream, req);

		boost::beast::flat_buffer buffer;

		boost::beast::http::response<boost::beast::http::dynamic_body> res;

		boost::beast::http::read(stream, buffer, res);

		string json = boost::beast::buffers_to_string(res.body().data());
		cout << json << endl;

		boost::beast::error_code ec;
		stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

		if (ec && ec != boost::beast::errc::not_connected)
		{
			clog << "error: " << ec.message() << endl;
			return -1;
		}
	}
	catch (std::exception const& ex) {
		clog << "exception: " << ex.what() << endl;

		return -1;
	}


	return 0;
}
