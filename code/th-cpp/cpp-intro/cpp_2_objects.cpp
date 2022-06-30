#include <iostream>
#include <string>

class Sensor {

public:
	Sensor(int id, const std::string &name) : id(id), name(name) {
		// ... Any code you need in the constructor goes here ...
		total_sensors++;
	}
	
	virtual ~Sensor() {
		// Do something in the destructor...
		total_sensors--;
	}
	
	const std::string &get_name() {
		return this->name;	// or this->id
	}
	
	void set_name(const std::string &name) {
		this->name = name;
	}
	
	int get_id() const;

	static int get_nr_sensors() {
		return total_sensors;
	}
	
//	virtual void update() = 0;
	
	virtual const char * get_class_name() const {
		return "Sensor";
	}

protected:
	std::string name;

private:
	int id;
	static int total_sensors;
};

int Sensor::total_sensors = 0;

int Sensor::get_id() const {
	return id;	// or this->id
}

class TemperatureSensor : public Sensor {
public:
	TemperatureSensor(int id, const std::string &name) : Sensor(id, name) {
		this->name = "[TEMP] " + this->name;
	}
	
/*	virtual void update() {
		// Read the temperature from the sensor and fill this->temperature...
		this->temperature = 20.5;
	}*/

	virtual const char * get_class_name() const {
		return "TemperatureSensor";
	}
	
	float get_temperature_C() const {
		return this->temperature;
	}

	float get_temperature_F() const {
		return (this->temperature * 9./5.) + 32.;
	}
	
private:
	float temperature;	// Saved in Celsius
};

class HumiditySensor : public Sensor {
public:
	HumiditySensor(int id, const std::string &name) : Sensor(id, name) {
		this->name = "[HUM] " + this->name;
	}

	virtual const char * get_class_name() const {
		return "HumiditySensor";
	}	

	float get_humidity() const {
		return humidity;
	}
private:
	float humidity;
};


int main() {

	Sensor s(123, "Generic");
	std::cout << "ID is: " << s.get_id() << "\tName is: " << s.get_name() << std::endl;

	Sensor            *s1 = new Sensor(123, "Generic");
	TemperatureSensor *s2 = new TemperatureSensor(123, "Room");
	HumiditySensor    *s3 = new HumiditySensor(123, "Corridor");
	
	std::cout << s1->get_class_name() << "\t" << s2->get_class_name() << "\t" << s3->get_class_name() << std::endl;

	// I can access s2->get_temperature_C() and s3->get_humidity()
	std::cout << s2->get_temperature_C() << "\t" << s3->get_humidity() << std::endl;

	Sensor *s4 = new Sensor(123, "Generic");
	Sensor *s5 = new TemperatureSensor(123, "Room");
	Sensor *s6 = new HumiditySensor(123, "Corridor");

	std::cout << s4->get_class_name() << "\t" << s5->get_class_name() << "\t" << s6->get_class_name() << std::endl;

	// I CANNOT access s5->get_temperature_C() or s6->get_humidity()
	//std::cout << s5->get_temperature_C() << "\t" << s6->get_humidity() << std::endl;


	std::cout << "Number of sensors: " << Sensor::get_nr_sensors() << std::endl;
	delete s1;
	delete s2;
	delete s3;
	delete s4;
	delete s5;
	delete s6;
	std::cout << "Number of sensors: " << Sensor::get_nr_sensors() << std::endl;

	return 0;
}

