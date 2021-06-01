//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#pragma once
#include <exception>
class MyException: public std::exception{
public:
	virtual const char *what(){
		return "Unknown Myexception.";
	}
} ;
class NonIntException: public MyException{
public:
	const char *what(){
		return "The number you read is supposed to be an integer, but in fact it's not.";
	}
} ;
class EofException: public MyException{
public:
	const char *what(){
		return "There should be more numbers, but we got an EOF.";
	}
} ;
class NullFileException: public MyException{
public:
	const char *what(){
		return "File not exists";
	}
} ;

class HardConstraintsException: public MyException{
public:
	virtual const char *what(){
		return "Route disobeys some hard constraints.";
	}
} ;
class InvalidVertexException: public HardConstraintsException{
public:
	const char *what(){
		return "Invalid vertex index or empty route.";
	}
} ;
class NoSuchEdgeException: public HardConstraintsException{
public:
	const char *what(){
		return "There's no such edge between two adjacent vertices.";
	}
} ;
class NotTourException: public HardConstraintsException{
public:
	const char *what(){
		return "The route is not a tour, which both starts and ends at Z.";
	}
} ;
class CustomerNotServedException: public HardConstraintsException{
public:
	const char *what(){
		return "There's a customer not served. Do you think that's valid but not feasible?";
	}
} ;
class LengthErrorException: public HardConstraintsException{
public:
	const char *what(){
		return "The vectors' lengthes do not correspond.";
	}
} ;
class NotInOrderException: public HardConstraintsException{
public:
	const char *what(){
		return "The take-off events must be given in order.";
	}
} ;
class InvalidCustomerException: public HardConstraintsException{
public:
	const char *what(){
		return "The customers' id is invalid.";
	}
} ;
class InvalidIndexException: public HardConstraintsException{
public:
	const char *what(){
		return "The take-off index is invalid.";
	}
} ;
class TooManyGoodsException: public HardConstraintsException{
public:
	const char *what(){
		return "Police! The truck is overloaded!";
	}
} ;
class TooManyTrucksException: public HardConstraintsException{
public:
	const char *what(){
		return "Ooops, we do not have that much trucks.";
	}
} ;

