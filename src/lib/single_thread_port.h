/**
 * \file single_thread_port.h
 *
 * \date 2010
 * \author yoyek
 *
 * \brief Template class for single robot communication port
 */

#ifndef SINGLE_THREAD_PORT_H_
#define SINGLE_THREAD_PORT_H_

#include <string>

namespace mrrocpp {
namespace lib {


class single_thread_port_interface
{
private:
	std::string name;
	bool new_data;

public:
	virtual bool is_new_data() = 0;

	single_thread_port_interface(std::string _name) :
		name(_name), new_data(false) {}

	std::string get_name()
	{
		return name;
	}

};

template<class T>
class single_thread_port : public single_thread_port_interface {

private:
	T data;


public:
	single_thread_port(std::string _name) :
		single_thread_port_interface(_name) {}

	void set(T& _data) {
		data = _data;
		new_data = true;
	}

	T get() {
		new_data = false;
		return data;
	}

	bool is_new_data() {
		return new_data;
	}

};

}
}

#endif /* SINGLE_THREAD_PORT_H_ */