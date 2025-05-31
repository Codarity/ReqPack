start:
	g++ -Isrc/main/include -I/home/leodora/.nxpm/packages/cpp/sol2/v3.2.2/include -I/home/leodora/.nxpm/packages/cpp/spdlog/v1.14.1/include -o build/main src/main/cpp/*.cpp /home/leodora/.nxpm/packages/cpp/spdlog/v1.14.1/cpp/spdlog.a -L/usr/lib64 -llua5.4

run:
	./build/main
