all: main DataGen MapView
main: main.o BaseData.o FastReader.o Route.o utils.o BaseSolC.o DroneGet.o AdjGet.o Init.o SolSet.o pdata.o config.o
	g++ -o main main.o BaseData.o FastReader.o Route.o utils.o BaseSolC.o DroneGet.o AdjGet.o Init.o SolSet.o pdata.o config.o
main.o: main.cpp Init.h AdjGet.h DroneGet.h SolSet.h BaseSolC.h Route.h BaseData.h utils.h config.h Exceptions.h pdata.h
	g++ -o main.o -c main.cpp -std=c++14
config.o: config.cpp config.h
	g++ -o config.o -c config.cpp -std=c++14
utils.o: utils.cpp utils.h Exceptions.h
	g++ -o utils.o -c utils.cpp -std=c++14
FastReader.o: FastReader.cpp FastReader.h Exceptions.h config.h utils.h
	g++ -o FastReader.o -c FastReader.cpp -std=c++14
BaseData.o: BaseData.cpp BaseData.h config.h utils.h FastReader.h
	g++ -o BaseData.o -c BaseData.cpp -std=c++14
Route.o: Route.cpp Route.h BaseData.h utils.h config.h Exceptions.h
	g++ -o Route.o -c Route.cpp -std=c++14
BaseSolC.o: BaseSolC.cpp BaseSolC.h Route.h BaseData.h utils.h config.h Exceptions.h pdata.h
	g++ -o BaseSolC.o -c BaseSolC.cpp -std=c++14
DroneGet.o: DroneGet.cpp DroneGet.h BaseSolC.h Route.h BaseData.h utils.h config.h Exceptions.h pdata.h
	g++ -o DroneGet.o -c DroneGet.cpp -std=c++14
AdjGet.o: AdjGet.cpp AdjGet.h BaseSolC.h Route.h BaseData.h utils.h config.h Exceptions.h pdata.h
	g++ -o AdjGet.o -c AdjGet.cpp -std=c++14
Init.o: Init.cpp Init.h BaseSolC.h Route.h BaseData.h utils.h config.h Exceptions.h pdata.h
	g++ -o Init.o -c Init.cpp -std=c++14
SolSet.o: SolSet.cpp SolSet.h BaseSolC.h Route.h BaseData.h utils.h config.h Exceptions.h pdata.h
	g++ -o SolSet.o -c SolSet.cpp -std=c++14
pdata.o: pdata.cpp pdata.h BaseData.h
	g++ -o pdata.o -c pdata.cpp -std=c++14
DataGen: DataGen.o BaseData.o FastReader.o utils.o
	g++ -o DataGen DataGen.o BaseData.o FastReader.o utils.o
DataGen.o: DataGen.cpp BaseData.h gconfig.h config.h utils.h
	g++ -o DataGen.o -c DataGen.cpp -std=c++14
MapView: MapView.o BaseData.o FastReader.o utils.o
	g++ -mwindows -o MapView MapView.o BaseData.o FastReader.o utils.o
MapView.o: MapView.cpp BaseData.h gconfig.h config.h utils.h
	g++ -o MapView.o -c MapView.cpp -std=c++14
RouteView.o: RouteView.cpp Route.h BaseData.h utils.h config.h Exceptions.h gconfig.h
	g++ -o RouteView.o -c RouteView.cpp -std=c++14
RouteView: RouteView.o BaseData.o FastReader.o utils.o Route.o
	g++ -mwindows -o RouteView RouteView.o BaseData.o FastReader.o utils.o Route.o