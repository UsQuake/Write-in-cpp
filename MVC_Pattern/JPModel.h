#pragma once
#include <string>

class JPModel
{
public:
	std::string RectalName;
	std::string Title;
	std::string Occupation;
	std::string Region;
	std::string EmployType;
	std::string Addition;

	unsigned int HourlyWage;
	unsigned int WorkingPeriod;
	unsigned int WorkingDay;

	unsigned int NumOfWorkDay;
	unsigned int WorkHours;
	unsigned int HolidayPay;



	//Unsearchable
	std::string CompanyID;
};