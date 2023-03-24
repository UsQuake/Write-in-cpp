#pragma once
#include <vector>

class CompanyModel
{
public:
	std::string PwHash;
	std::string Name;
	std::string PhoneNumber;
	std::string Email;
	std::string BuisnessId;
	std::vector<unsigned int> AppliedCVIds;
	std::vector<unsigned int> MyJPIds;
};

