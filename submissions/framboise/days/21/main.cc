#include <algorithm>
#include <set>
#include <map>
#include <regex>
#include "../../utils.hh"

struct Food {
	std::set<std::string> ingredients;
	std::set<std::string> allergens;
};

int main () {
	// allergen id -> common ingredients 
	std::map<std::string, std::set<std::string>> all_allergens;
	std::map<std::string, u32> inert_ingredients;

	std::vector<Food> foods;
	foods.reserve(1000);
	std::regex regfood("(.*) \\(contains (.*)\\)");
	std::regex regingr("([a-z]+)(?: (.*))?");
	std::regex regalle("([a-z]+)(?:, (.*))?");
	std::smatch sma;
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.empty())
			break;
		if (!std::regex_match(line, sma, regfood))
			return 1;
		foods.push_back(Food{{}, {}});
		auto& food = foods.back();
		std::string ingrs = sma[1].str();
		std::string alles = sma[2].str();
		while (true) {
			if (!std::regex_match(ingrs, sma, regingr))
				return 2;
			food.ingredients.insert(sma[1].str());
			++inert_ingredients[sma[1].str()];
			if (!sma[2].matched)
				break;
			ingrs = sma[2].str();
		}
		while (true) {
			if (!std::regex_match(alles, sma, regalle))
				return 3;
			food.allergens.insert(sma[1].str());
			if (!sma[2].matched)
				break;
			alles = sma[2].str();
		}
	}

	for (auto& food : foods) {
		for (auto& alle : food.allergens) {
			auto& common = all_allergens[alle];
			if (common.empty()) {
				common.insert(food.ingredients.begin(), food.ingredients.end());
			} else {
				std::set<std::string> inter;
				std::set_intersection(common.begin(), common.end(),
				                      food.ingredients.begin(), food.ingredients.end(),
				                      std::inserter(inter, inter.end()));
				std::swap(inter, common);
				if (common.empty())
					common.insert("-");
			}
		}
	}

	for (auto& [allergen, common] : all_allergens) {
		std::cerr << allergen << ": ";
		for (auto& ingredient : common) {
			std::cerr << ingredient << " ";
			inert_ingredients.erase(ingredient);
		}
		std::cerr << std::endl;
	}

	u32 count = 0;
	for (auto& [ingredient, ingr_count] : inert_ingredients)
		count += ingr_count;
	std::cout << count << std::endl;

	std::cerr << "----" << std::endl;

	std::map<std::string, std::string> allergen_to_ingredient;

	while (all_allergens.size()) {
		for (auto& [allergen, common] : all_allergens) {
			if (common.size() == 1) {
				std::string ingredient = *common.begin();
				allergen_to_ingredient[allergen] = ingredient;
				all_allergens.erase(allergen);
				for (auto& [allergen_, common_] : all_allergens)
					common_.erase(ingredient);
				break;
			}
		}
	}

	bool first_ingr = true;
	for (auto& [allergen, ingredient] : allergen_to_ingredient) {
		if (!first_ingr)
			std::cout << ',';
		first_ingr = false;
		std::cout << ingredient;
	}
	std::cout << std::endl;

	return 0;
}
