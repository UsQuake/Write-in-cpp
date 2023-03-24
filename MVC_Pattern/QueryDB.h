#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include <fstream>
#include <iostream>
#include "CompanyModel.h"
#include "IndividualModel.h"
#include "CVModel.h"
#include "JPModel.h"
namespace alba_java
{
	void LOG_ALBA_JAVA_METHOD(std::string method_name);


	enum JPSearchOption
	{
		RECTAL_NAME, TITLE, OCCUPATION, REGION, EMPLOYMENT_TYPE, ADDITIONAL_INFO,
		//String Type Search Options
		HOURLY_WAGE, WORKING_PERIOD, WORKING_DAYS,
		NUM_OF_WORK_DAYS, WORKING_HOURS, HOLIDAY_PAY
		//Range Type Search Options
	};
	enum CVSearchOption
	{
		CV_NAME, WORKER_NAME, WORKER_GENDER,
		WORK_PLACE, WORKER_EDUCATION, WORKER_INTRODUCTION,
		WORKER_AGE
	};
	class QueryDB
	{
	private:
		std::unordered_map<std::string, IndividualModel> mIndividuals;
		std::unordered_map<std::string, CompanyModel> mCompanies;
		std::vector<CVModel> mCVs;
		std::vector<JPModel> mJPs;
		QueryDB() {}
		~QueryDB() {}
		void deleteReferencesOfDeletedCV(unsigned int cv_id);
		void deleteReferencesOfDeletedJP(unsigned int jp_id);
		static void deleteRefrencesInVector(std::vector<unsigned int>& index_vec, unsigned int elem);
		static bool isExistKeywordInString(const std::string& given_string, const std::string& keyword);
	public:
		QueryDB(const QueryDB& other) = delete;
		static QueryDB* GetInstance();
		//Singleton Pattern; <<singleton>>


		bool IsIdAlreadyExist(const std::string& id);
		bool IsIndividual(const std::string& id, const std::string& pw_hash);
		bool IsCompany(const std::string& id, const std::string& pw_hash);

		bool CreateAccountOrFalse(const std::string& id, const IndividualModel& indi);
		bool CreateAccountOrFalse(const std::string& id, const CompanyModel& comp);
		CompanyModel* ReadCompanyOrNull(const std::string& id);
		IndividualModel* ReadIndividualOrNull(const std::string& id);
		bool UpdateAccountOrFalse(const std::string& id, const IndividualModel& indi);
		bool UpdateAccountOrFalse(const std::string& id, const CompanyModel& comp);
		bool DeleteAccount(const std::string& id);

		const std::map<unsigned int, CVModel> SearchCV(const std::string& keyword, unsigned int option);
		const std::map<unsigned int, CVModel> SearchCVInRange(unsigned int option, unsigned int start, unsigned int end);
		const std::map<unsigned int, JPModel> SearchJP(const std::string& keyword, unsigned int option);
		const std::map<unsigned int, JPModel> SearchJPInRange(unsigned int option, unsigned int start, unsigned int end);

		unsigned int GetJPCount();
		void CreateJP(const JPModel& jp);
		JPModel* ReadJPOrNull(unsigned int jp_id); //should test heap memory leak.(life-time)
		bool UpdateJPOrFalse(unsigned int jp_id, const JPModel& jp);
		bool DeleteJPOrFalse(unsigned int jp_id); // should test no refer.


		unsigned int GetCVCount();
		void CreateCV(const CVModel& cv);
		CVModel* ReadCVOrNull(unsigned int cv_id); //should test heap memory leak.(life-time)
		bool UpdateCVOrFalse(unsigned int cv_id, const CVModel& cv);
		bool DeleteCVOrFalse(unsigned int cv_id);// should test no refer.
	};
}
