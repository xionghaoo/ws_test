// ws_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <uwebsockets/App.h>
#include <iostream>
#include <thread>

uWS::WebSocket<false, true, int>* pws;
//uWS::WebSocket<false, true, int>* pws;

// auto loop_ = uWS::Loop::get();
std::shared_ptr<std::thread> thread_;

void run(void (*fp)(int, int)) {
	for (size_t i = 0; i < 10000; i++)
	{
		if (!pws)
		{
			break;
		}
		fp(i, 0);
		std::cout << "count " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void thread_send() {
	thread_ = std::make_shared<std::thread>(run, [](int a, int b) {

		//loop_->defer([a]() { });
		std::cout << "thread: " << std::this_thread::get_id() << std::endl;
		/* uWs::App of each worker. uWs::App object has knowledge of all socket contexts. */
		//w.app_->publish("broadcast", message, opCode);
		if (pws)
		{
			pws->send("thread send" + std::to_string(a), uWS::OpCode::TEXT);

		}
		});


}

int main()
{
	/* ws->getUserData returns one of these */
	struct PerSocketData {
		/* Fill with user data */
	};
	std::cout << "main: " << std::this_thread::get_id() << std::endl;
	/* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
	 * You may swap to using uWS:App() if you don't need SSL */
	uWS::App().ws<PerSocketData>("/*", {
		/* Settings */
		.compression = uWS::CompressOptions(uWS::DEDICATED_COMPRESSOR_4KB | uWS::DEDICATED_DECOMPRESSOR),
		.maxPayloadLength = 100 * 1024 * 1024,
		.idleTimeout = 16,
		.maxBackpressure = 100 * 1024 * 1024,
		.closeOnBackpressureLimit = false,
		.resetIdleTimeoutOnSend = false,
		.sendPingsAutomatically = true,
		/* Handlers */
		.upgrade = nullptr,
		.open = [](auto* ws) {
			/* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
			pws = (uWS::WebSocket<false, true, int>*)ws;
			std::cout << "open ws: " << std::this_thread::get_id() << std::endl;
			pws->send("open", uWS::OpCode::TEXT);
			thread_send();
		},
		.message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
			/* This is the opposite of what you probably want; compress if message is LARGER than 16 kb
			 * the reason we do the opposite here; compress if SMALLER than 16 kb is to allow for
			 * benchmarking of large message sending without compression */
			std::cout << "thread: " << std::this_thread::get_id() << std::endl;
			pws->send(message, opCode, message.length() < 16 * 1024);
		},
		.drain = [](auto* ws) {
			/* Check ws->getBufferedAmount() here */
			std::cout << "buffer size: " << ws->getBufferedAmount() << std::endl;
		},
		.ping = [](auto*/*ws*/, std::string_view) {
			/* Not implemented yet */
		},
		.pong = [](auto*/*ws*/, std::string_view) {
			/* Not implemented yet */
		},
		.close = [](auto*/*ws*/, int /*code*/, std::string_view /*message*/) {
			pws = nullptr;
			thread_->join();
			  std::cout << "close" << std::endl;
			  /* You may access ws->getUserData() here */
		  }
		}).listen(9001, [](auto* listen_socket) {
			  if (listen_socket) {
				  std::cout << "Listening on port " << 9001 << std::endl;
			  }
			}).run();

}