#include "object_holder.h"
#include "object.h"
#include <sstream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
using namespace   Catch::Matchers;


using namespace std;

namespace Runtime {

	class Logger : public Object {
	public:
		static int instance_count;

		explicit Logger(int value = 0) : id(value) {
			++instance_count;
		}

		Logger(const Logger& rhs) : id(rhs.id)
		{
			++instance_count;
		}

		Logger(Logger&& rhs) : id(rhs.id)
		{
			++instance_count;
		}

		int GetId() const { return id; }

		~Logger() {
			--instance_count;
		}

		void Print(ostream& os) override {
			os << id;
		}

	private:
		int id;
	};

	int Logger::instance_count = 0;

	TEST_CASE("ParaPython Nonowning", "[ParaPython]") {
		CHECK(Logger::instance_count == 0);
		Logger logger(784);
		{
			auto oh = ObjectHolder::Share(logger);
			CHECK(oh);
		}
		CHECK(Logger::instance_count == 1);

		auto oh = ObjectHolder::Share(logger);
		CHECK(oh);
		CHECK(oh.Get() == &logger);

		ostringstream os;
		oh->Print(os);
		CHECK(os.str() == "784");
	}
	TEST_CASE("ParaPython Owning", "[ParaPython]")
	{
		CHECK(Logger::instance_count == 0);
		{
			auto oh = ObjectHolder::Own(Logger());
			CHECK(oh);
			CHECK(Logger::instance_count == 1);
		}
		CHECK(Logger::instance_count == 0);

		auto oh = ObjectHolder::Own(Logger(312));
		CHECK(oh);
		CHECK(Logger::instance_count == 1);

		ostringstream os;
		oh->Print(os);
		CHECK(os.str() == "312");

	}
	TEST_CASE("ParaPython TestMove", "[ParaPython]")
	{
		{
			CHECK(Logger::instance_count == 0);
			Logger logger;

			auto one = ObjectHolder::Share(logger);
			ObjectHolder two = std::move(one);

			CHECK(Logger::instance_count == 1);
			CHECK(two.Get() == &logger);
		}
		{
			CHECK(Logger::instance_count == 0);
			auto one = ObjectHolder::Own(Logger());
			CHECK(Logger::instance_count == 1);
			Object* stored = one.Get();
			ObjectHolder two = std::move(one);
			CHECK(Logger::instance_count == 1);

			CHECK(two.Get() == stored);
			CHECK(!one);
		}
	}
	TEST_CASE("ParaPython Nullptr", "[ParaPython]")
	{
		ObjectHolder oh;
		CHECK(!oh);
		CHECK(!oh.Get());
	}



} /* namespace Runtime */
