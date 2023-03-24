#pragma once
#include <string>
#include <vector>

class IndividualModel
{
public:
	std::string PwHash;
	std::string Name;
	std::string PhoneNumber;
	std::string Birthday;
	std::string Email;
	std::string PostalCode;
	std::string ResidenceAddress;
	std::vector<unsigned int> MyCVIds;
};