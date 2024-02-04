#pragma once

#include <string>

class Action
{
private:
	std::string m_name = "NONE"; // Ex: "SHOOT", "JUMP", "RIGHT"
	std::string m_type = "NONE"; // "START" or "END"

public:
	Action();
	Action(const std::string& name, const std::string& type);

	const std::string& name() const;
	const std::string& type() const;
};