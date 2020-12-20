#include <sstream>
#include <regex>
#include "../../utils.hh"

struct Field {
	std::string name;
	std::vector<std::pair<u32, u32>> ranges;
};

void make_ticket (std::vector<u32>& ticket, const std::string& line) {
	std::stringstream line_sstr(line);
	std::string element;
	while (true) {
		if (!std::getline(line_sstr, element, ','))
			break;
		ticket.push_back(std::stoi(element));
	}
};

int main () {
	std::vector<Field> fields;
	fields.reserve(100);
	std::vector<u32> my_ticket;
	std::vector<std::vector<u32>> other_tickets;
	other_tickets.reserve(1000);
	std::string line;
	std::regex field_reg("([a-z ]+): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)");
	std::smatch sma;
	while (true) {
		std::getline(std::cin, line);
		if (line.empty())
			break;
		if (!std::regex_match(line, sma, field_reg))
			return 1;
		Field f;
		f.name = sma[1].str();
		for (int i = 0; i < 2; ++i)
			f.ranges.push_back(std::make_pair(std::stoi(sma[2 + 2 * i].str()),
			                                  std::stoi(sma[3 + 2 * i].str())));
		fields.emplace_back(std::move(f));
	}
	std::getline(std::cin, line); // "your ticket:"
	std::getline(std::cin, line);
	make_ticket(my_ticket, line);
	std::getline(std::cin, line); // empty
	std::getline(std::cin, line); // "nearby tickets:"
	while (true) {
		std::getline(std::cin, line);
		if (line.empty())
			break;
		std::vector<u32> ticket;
		make_ticket(ticket, line);
		other_tickets.emplace_back(std::move(ticket));
	}

	std::vector<std::vector<u32>*> valid_tickets;
	valid_tickets.reserve(other_tickets.size());

	auto validate = [&] (u32 tval) {
		u64 vmask = 0;
		for (int i = 0; i < fields.size(); ++i)
			for (auto& [inf, sup] : fields[i].ranges)
				if (inf <= tval && tval <= sup)
					vmask |= ((u64)1 << i);
		return vmask;
	};

	u32 err = 0;
	for (auto& ticket : other_tickets) {
		bool ticket_valid = true;
		for (u32 tval : ticket) {
			if (!validate(tval)) {
				err += tval;
				ticket_valid = false;
			}
		}
		if (ticket_valid)
			valid_tickets.push_back(&ticket);
	}
	std::cout << err << std::endl;

	std::vector<u64> entry_masks(my_ticket.size());
	for (int i = 0; i < my_ticket.size(); ++i)
		entry_masks[i] = validate(my_ticket[i]);
	for (auto& ticket : valid_tickets)
		for (int i = 0; i < ticket->size(); ++i)
			entry_masks[i] &= validate((*ticket)[i]);

	bool field_found = true;
	while (field_found) {
		field_found = false;
		for (u64& ref_mask : entry_masks) {
			u64 mask = ref_mask;
			if(std::popcount(mask) == 1) {
				for (u64& other_mask : entry_masks)
					other_mask &= ~mask;
				ref_mask = mask | ((u64)1 << 63);
				field_found = true;
			}
		}
	}

	u64 departures = 1;
	for (int i = 0; i < my_ticket.size(); ++i) {
		//std::cerr << "field #" << i;
		for (int k = 0; k < fields.size(); ++k)
			if ((entry_masks[i] >> k) & 1) {
				//std::cerr << " | " << fields[k].name;
				if (fields[k].name.starts_with("departure "))
					departures *= my_ticket[i];
			}
		//std::cerr << std::endl;
	}

	std::cout << departures << std::endl;

	return 0;
}
