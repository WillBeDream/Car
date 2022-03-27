// Car.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<conio.h>
#include<thread>

using std::cout;
using std::cin;
using std::endl;
using namespace std::chrono_literals;

#define Escape 27
#define Enter 13

#define MIN_TANK_VOLUME 20
#define MAX_TANK_VOLUME 80

#define MIN_ENGINE_CONSUMPTION 3
#define MAX_ENGINE_CONSUMPTION 25

class Tank
{
	const unsigned int VOLUME;
	double fuel_level;
public:
	unsigned int get_VOLUME()const
	{
		return this->VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	void fill(double fuel_level)
	{
		if (fuel_level <= 0)return;
		if (this->fuel_level + fuel_level <= VOLUME)this->fuel_level += fuel_level;
		else this->fuel_level = VOLUME;
	}

	double give_fuel(double amount)
	{
		if (fuel_level - amount > 0)fuel_level -= amount;
		else fuel_level = 0;
		return fuel_level;
	}

	Tank(const unsigned int volume):
		VOLUME(volume>= MIN_TANK_VOLUME && volume <= MAX_TANK_VOLUME?volume:60)
	{
		this->fuel_level = 0;
		cout << "Tank is ready" << endl;
	}
	~Tank()
	{
		cout << "Tank is over" << endl;
	};

	void info()const
	{
		cout << "Tank volemt: " << VOLUME << " liters\n";
		cout<<"Fuel level: "<< fuel_level<< " liters\n";
	}

};

class Engine
{
	double consumption;
	double consumption_per_second;
	bool is_started;

public:
	double get_consumption()const
	{
		return consumption;
	}
	double get_consumption_per_second()const
	{
		return consumption_per_second;
	}
	void set_consumption(double consumption)
	{	
		if (consumption >= MIN_ENGINE_CONSUMPTION && consumption <= MAX_ENGINE_CONSUMPTION)
			this->consumption = consumption;
		else
			this->consumption = 30;
		this->consumption_per_second = this->consumption * 3e-5;
	}
	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()const
	{
		return is_started;
	}
	void info()const
	{
		cout << "Consumption per 100km: " << consumption << " liters\n";
		cout << "Consumption per second: " << consumption_per_second << " liters\n";
	}
	Engine(double consumption)
	{
		set_consumption(consumption);
		is_started = false;
		cout << "Engine is ready" << endl;
	}
	~Engine()
	{
		cout << "Engine is over" << endl;
	};

};

class Car
{
	Engine engine;
	Tank tank;
	bool driver_inside;
	double speed = 0;
	struct Control
	{
		std::thread panel_thread;
		std::thread engine_idle_thread;
		std::thread free_wheeling_thread;
		std::thread speed_idle_thread;
	}control;
public:

	Car(double engine_consumption, unsigned int tank_volume)
		:engine(engine_consumption), tank(tank_volume)
	{
		driver_inside = false;
		cout << "Your car is ready" << endl;
	}
	~Car()
	{
		cout << "Car is over"<<endl;
	};

	double get_speed()const
	{
		return this->speed;
	}

	void get_in()
	{
		driver_inside = true;
		/*panel();*/
		control.panel_thread = std::thread(&Car::panel, this);
	}

	void get_out()
	{
		driver_inside = false;
		if(control.panel_thread.joinable())control.panel_thread.join();
		system("CLS");
		cout << "You are out of your car" << endl;
	}

	void start_engine()
	{
		if (driver_inside && tank.get_fuel_level())
		{
			engine.start();
			control.engine_idle_thread = std::thread(&Car::engine_idle, this);
		}
	}

	void stop_engine()
	{
		engine.stop();
		if (control.engine_idle_thread.joinable())control.engine_idle_thread.join();
	}

	double speed_down()
	{
		return this->speed -= 0,2;
	}
	
	void start_speed()
	{
		if (driver_inside && tank.get_fuel_level())
		{
			engine.start();
			control.speed_idle_thread = std::thread(&Car::speed_ide, this);
		}
	}


	void control_car()
	{
		cout << "press enter to get in" << endl;
		char key;
		do
		{
			key = _getch();
			switch (key)
			{
			case 'I':
			case 'i':
				if (engine.started())stop_engine();
				else start_engine();
				break;
			case Enter:
				if (driver_inside)get_out();
				else get_in();
				break;
			case 'F':
			case 'f' :
				if (driver_inside)
				{
					cout << "ƒл€ того чтобы заправитьс€, нужно выйти из машины" << endl;
				}
				else
				{
					int amount;
					cout << "введите литраж: "; cin >> amount;
					tank.fill(amount);
					cout << "заправка успешно завершена";
					break;
				}
			case 'W':
			case 'w':
				if(driver_inside && engine.started())
					this->speed += 10;
				break;
			case Escape:
				stop_engine();
				get_out();
				break;
			}
			
		} while (key != Escape);
	}
	void panel()const
	{
		while (driver_inside)
		{
			system("CLS");
			cout << "Fuel level: " << tank.get_fuel_level() << " liters\n";
			cout << "Engine is: " << (engine.started() ? "started" : "stopped") << endl;
			cout << "Speed car: " << speed << " kilometers/hour" << endl;
			if (tank.get_fuel_level() <= 5)cout << "LOW FUEL" << endl;
			std::this_thread::sleep_for(2s);
		}
	}

	void engine_idle()
	{
		while (engine.started() && tank.give_fuel(engine.get_consumption_per_second()))
		{
			std::this_thread::sleep_for(1s);
		}
	}

	void speed_ide()
	{
		while (engine.started() && speed_down())
		{
			std::this_thread::sleep_for(1s);
		}
	}

	void info()const
	{
		engine.info();
		tank.info();
	}
};



//#define TANK_CHECK
//#define ENGINE_CHECK

int main()
{
	setlocale(LC_ALL, "");
#ifdef TANK_CHECK
	Tank tank(40);
	tank.info();
	int fuel;
	while (true)
	{
		cout << "¬ведите обьем топлива: ";
		cin >> fuel;
		tank.fill(fuel);
		tank.info();
	}
#endif // TANK_CHECK

#ifdef ENGINE_CHECK
	Engine engine(10);
	engine.info();
#endif // ENGINE_CHECK

	Car bmw(12, 60);
	/*bmw.info();*/
	bmw.control_car();


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
