#include "QueryDB.h"

namespace alba_java
{
	void LOG_ALBA_JAVA_METHOD(std::string method_name)
	{
		std::cout << "\n\033[91m Call " + method_name + "()\033[0m" << std::endl;
	}
	QueryDB* QueryDB::GetInstance()
	{
		LOG_ALBA_JAVA_METHOD("GetInstance");
		static QueryDB instance;
		return &instance;
	}
	bool QueryDB::isExistKeywordInString(const std::string& given_string, const std::string& keyword)
	{
		LOG_ALBA_JAVA_METHOD("isExistKeywordInString");
		if (given_string.length() < keyword.length())
		{
			return false;
		}
		if (keyword.length() == 0)
		{
			return true;
		}

		char prefix = keyword[0];
		size_t keyword_last_index = keyword.length() - 1;
		char suffix = keyword[keyword_last_index];
		for (size_t i = 0; i <= given_string.length() - keyword.length(); i++)
		{
			if (given_string[i] == prefix && given_string[i + keyword_last_index] == suffix)
			{

				for (size_t j = 0; j <= keyword.length() / 2; j++)
				{
					if (given_string[i + j] != keyword[j] || given_string[i + keyword_last_index - j] != keyword[keyword_last_index - j])
					{
						return false;
					}
				}
				return true;
			}
		}
		return false;
	}
	unsigned int QueryDB::GetJPCount()
	{
		LOG_ALBA_JAVA_METHOD("GetJPCount");
		return mJPs.size();
	}
	bool QueryDB::IsIdAlreadyExist(const std::string& id)
	{
		LOG_ALBA_JAVA_METHOD("IsIdAlreadyExist");
		if (mCompanies.find(id) == mCompanies.end() && mIndividuals.find(id) == mIndividuals.end())
			return false;
		return true;
	}

	bool QueryDB::IsCompany(const std::string& id, const std::string& pw_hash)
	{
		LOG_ALBA_JAVA_METHOD("IsCompany");
		if (mCompanies.find(id) == mCompanies.end())
			return false;

		if (mCompanies[id].PwHash != pw_hash)
			return false;

		return true;
	}

	bool QueryDB::IsIndividual(const std::string& id, const std::string& pw_hash)
	{
		LOG_ALBA_JAVA_METHOD("IsIndividual");
		if (mIndividuals.find(id) == mIndividuals.end())
			return false;

		if (mIndividuals[id].PwHash != pw_hash)
			return false;
		return true;
	}

	bool QueryDB::CreateAccountOrFalse(const std::string& id, const IndividualModel& indi)
	{
		LOG_ALBA_JAVA_METHOD("CreateAccountOrFalse");
		if (IsIdAlreadyExist(id))
			return false;
		mIndividuals[id] = indi;

		return true;
	}

	bool QueryDB::CreateAccountOrFalse(const std::string& id, const CompanyModel& comp)
	{
		LOG_ALBA_JAVA_METHOD("CreateAccountOrFalse");
		if (IsIdAlreadyExist(id))
			return false;
		mCompanies[id] = comp;
		return true;
	}

	CompanyModel* QueryDB::ReadCompanyOrNull(const std::string& id)
	{
		LOG_ALBA_JAVA_METHOD("ReadCompanyOrNull");
		if (!IsIdAlreadyExist(id))
			return nullptr;
		else
		{
			return new CompanyModel(mCompanies[id]);
		}
	}

	IndividualModel* QueryDB::ReadIndividualOrNull(const std::string& id)
	{
		LOG_ALBA_JAVA_METHOD("ReadIndividualOrNull");
		if (!IsIdAlreadyExist(id))
			return nullptr;
		else
		{
			return new IndividualModel(mIndividuals[id]);
		}
	}

	bool QueryDB::UpdateAccountOrFalse(const std::string& id, const IndividualModel& indi)
	{
		LOG_ALBA_JAVA_METHOD("UpdateAccountOrFalse");
		if (!IsIdAlreadyExist(id))
			return false;
		mIndividuals[id] = indi;
		return true;
	}
	bool QueryDB::UpdateAccountOrFalse(const std::string& id, const CompanyModel& comp)
	{
		LOG_ALBA_JAVA_METHOD("UpdateAccountOrFalse");
		if (!IsIdAlreadyExist(id))
			return false;
		mCompanies[id] = comp;

		return true;
	}
	bool QueryDB::DeleteAccount(const std::string& id)
	{
		LOG_ALBA_JAVA_METHOD("DeleteAccount");
		if (!IsIdAlreadyExist(id))
			return false;

		if (mCompanies.find(id) == mCompanies.end())
		{
			//If doesn't exist in Company, It should in Individual.
			for (const auto& cv_id : mIndividuals[id].MyCVIds)
			{
				mCVs.erase(mCVs.begin() + cv_id);
			}
			mIndividuals.erase(id);
		}


		if (mIndividuals.find(id) == mIndividuals.end())
		{
			//If doesn't exist in Individual, It should in Company.

			for (const auto& jp_id : mCompanies[id].MyJPIds)
			{
				mJPs.erase(mJPs.begin() + jp_id);
			}
			mCompanies.erase(id);
		}

		return true;
	}


	const std::map<unsigned int, CVModel> QueryDB::SearchCV(const std::string& keyword, unsigned int opt)
	{
		LOG_ALBA_JAVA_METHOD("SearchCV");
		std::map<unsigned int, CVModel> tmp;
		unsigned int reference_index = 0;
		for (const auto& cv : mCVs)
		{
			if (opt == CV_NAME && isExistKeywordInString(cv.CVName, keyword))
			{
				tmp[reference_index] = cv;
			}

			else if (opt == WORKER_EDUCATION && isExistKeywordInString(cv.WorkerEducation, keyword))
			{
				tmp[reference_index] = cv;
			}
			else if (opt == WORKER_GENDER && isExistKeywordInString(cv.WorkerGender, keyword))
			{
				tmp[reference_index] = cv;
			}
			else if (opt == WORKER_INTRODUCTION && isExistKeywordInString(cv.WorkerIntroduction, keyword))
			{
				tmp[reference_index] = cv;
			}
			else if (opt == WORKER_NAME && isExistKeywordInString(cv.WorkerName, keyword))
			{
				tmp[reference_index] = cv;
			}
			reference_index++;
		}
		return tmp;
	}
	const  std::map<unsigned int, CVModel> QueryDB::SearchCVInRange(unsigned int opt, unsigned int start, unsigned int end)
	{
		LOG_ALBA_JAVA_METHOD("SearchCVInRange");
		std::map<unsigned int, CVModel> tmp;
		unsigned int reference_index = 0;
		for (const auto& cv : mCVs)
		{
			if (opt == WORKER_AGE && (start <= cv.WorkerAge && cv.WorkerAge <= end))
			{
				tmp[reference_index] = cv;
			}
			reference_index++;
		}
		return tmp;
	}
	const std::map<unsigned int, JPModel> QueryDB::SearchJP(const std::string& keyword, unsigned int opt)
	{
		LOG_ALBA_JAVA_METHOD("SearchJP");
		std::map<unsigned int, JPModel> tmp;
		unsigned int reference_index = 0;
		for (const auto& jp : mJPs)
		{
			if (opt == RECTAL_NAME && isExistKeywordInString(jp.RectalName, keyword))
			{
				tmp[reference_index] = jp;
			}
			if (opt == TITLE && isExistKeywordInString(jp.Title, keyword))
			{
				tmp[reference_index] = jp;
			}
			if (opt == OCCUPATION && isExistKeywordInString(jp.Occupation, keyword))
			{
				tmp[reference_index] = jp;
			}
			if (opt == REGION && isExistKeywordInString(jp.Region, keyword))
			{
				tmp[reference_index] = jp;
			}
			if (opt == EMPLOYMENT_TYPE && isExistKeywordInString(jp.EmployType, keyword))
			{
				tmp[reference_index] = jp;
			}
			if (opt == ADDITIONAL_INFO && isExistKeywordInString(jp.Addition, keyword))
			{
				tmp[reference_index] = jp;
			}
			reference_index++;
		}
		return tmp;
	}

	const  std::map<unsigned int, JPModel> QueryDB::SearchJPInRange(unsigned int opt, unsigned int start, unsigned int end)
	{
		LOG_ALBA_JAVA_METHOD("SearchJPInRange");
		std::map<unsigned int, JPModel> tmp;
		unsigned int reference_index = 0;
		for (const auto& jp : mJPs)
		{
			if (opt == HOURLY_WAGE && (start <= jp.HourlyWage && jp.HourlyWage <= end))
			{
				tmp[reference_index] = jp;
			}
			if (opt == WORKING_PERIOD && (start <= jp.WorkingPeriod && jp.WorkingPeriod <= end))
			{
				tmp[reference_index] = jp;
			}
			if (opt == WORKING_DAYS && (start <= jp.WorkingDay && jp.WorkingDay <= end))
			{
				tmp[reference_index] = jp;
			}
			if (opt == NUM_OF_WORK_DAYS && (start <= jp.NumOfWorkDay && jp.NumOfWorkDay <= end))
			{
				tmp[reference_index] = jp;
			}
			if (opt == WORKING_HOURS && (start <= jp.WorkHours && jp.WorkHours <= end))
			{
				tmp[reference_index] = jp;
			}
			if (opt == HOLIDAY_PAY && (start <= jp.HolidayPay && jp.HolidayPay <= end))
			{
				tmp[reference_index] = jp;
			}

			reference_index++;
		}
		return tmp;
	}

	void QueryDB::CreateJP(const JPModel& jp)
	{
		LOG_ALBA_JAVA_METHOD("CreateJP");
		mJPs.push_back(jp);
	}
	JPModel* QueryDB::ReadJPOrNull(unsigned int jp_id)
	{
		LOG_ALBA_JAVA_METHOD("ReadJPOrNull");
		if (jp_id < mJPs.size())
		{
			return new JPModel(mJPs[jp_id]);
		}
		return nullptr;
	}
	bool QueryDB::UpdateJPOrFalse(unsigned int jp_id, const JPModel& jp)
	{
		LOG_ALBA_JAVA_METHOD("UpdateJPOrFalse");
		if (jp_id < mJPs.size())
		{
			mJPs[jp_id] = jp;
			return true;
		}
		return false;
	}
	bool QueryDB::DeleteJPOrFalse(unsigned int jp_id)
	{
		LOG_ALBA_JAVA_METHOD("DeleteJPOrFalse");
		if (jp_id >= mJPs.size())
			return false;

		auto iter = mJPs.begin();

		iter += jp_id;

		mJPs.erase(iter);

		deleteReferencesOfDeletedJP(jp_id);

		return true;
	}


	void QueryDB::CreateCV(const CVModel& cv)
	{
		LOG_ALBA_JAVA_METHOD("CreateCV");
		mCVs.push_back(cv);
	}
	unsigned int  QueryDB::GetCVCount()
	{
		LOG_ALBA_JAVA_METHOD("GetCVCount");
		return mCVs.size();
	}
	CVModel* QueryDB::ReadCVOrNull(unsigned int cv_id)
	{
		LOG_ALBA_JAVA_METHOD("ReadCVOrNull");
		if (cv_id < mCVs.size())
		{
			return new CVModel(mCVs[cv_id]);
		}
		return nullptr;
	}
	bool QueryDB::UpdateCVOrFalse(unsigned int cv_id, const CVModel& cv)
	{
		LOG_ALBA_JAVA_METHOD("UpdateCVOrFalse");
		if (cv_id < mCVs.size())
		{
			mCVs[cv_id] = cv;
			return true;
		}
		return false;
	}
	bool QueryDB::DeleteCVOrFalse(unsigned int cv_id)
	{
		LOG_ALBA_JAVA_METHOD("DeleteCVOrFalse");
		if (cv_id >= mCVs.size())
			return false;

		auto iter = mCVs.begin();

		iter += cv_id;

		mCVs.erase(iter);

		deleteReferencesOfDeletedCV(cv_id);

		return true;
	}


	void QueryDB::deleteReferencesOfDeletedCV(unsigned int cv_id)
	{
		LOG_ALBA_JAVA_METHOD("deleteReferencesOfDeletedCV");
		for (auto& comp : mCompanies)
		{
			deleteRefrencesInVector(comp.second.AppliedCVIds, cv_id);
		}

		for (auto& indi : mIndividuals)
		{
			deleteRefrencesInVector(indi.second.MyCVIds, cv_id);
		}
	}
	void QueryDB::deleteReferencesOfDeletedJP(unsigned int jp_id)
	{
		LOG_ALBA_JAVA_METHOD("deleteReferencesOfDeletedJP");
		for (auto& comp : mCompanies)
		{
			deleteRefrencesInVector(comp.second.MyJPIds, jp_id);
		}
	}

	void QueryDB::deleteRefrencesInVector(std::vector<unsigned int>& inVec, unsigned int elem)
	{
		LOG_ALBA_JAVA_METHOD("deleteRefrencesInVector");
		for (auto iter = inVec.begin(); iter != inVec.end(); ++iter)
		{
			if (*iter > elem)
				*iter = *iter - 1;
			if (*iter == elem)
				iter = inVec.erase(iter);
			if (iter == inVec.end())
				break;
		}
	}


}
