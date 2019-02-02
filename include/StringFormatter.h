#ifndef STRINGFORMATTER_H
#define STRINGFORMATTER_H

#include <string>
#include <vector>

/*
    %own_power_out% - how much power a component generates on its own (excluding other component on grid, but including bonuses)
    %own_heat_out% - same but for heat
    %power_out% - power being produced by component including all factors
    %heat_out% - heat being produced by component including all factors
    %lifespan% - lifespan of the component
    %life_left% - left life
    %life_passed% - passed life
    %stored_heat% - amount of stored heat
    %heat_capacity% - max amount of heat in the component (including all bonuses)
    %reactor_heat_capacity% - capacity of heat added to the reactor
    %reactor_power_capacity% - capacity of power added to the reactor
    %power_out_mul% - power generation multiplier for near components (not in percent, just a value)
    %heat_out_mul% - heat generation multiplier for near components (not in percent, just a value)
    %name% - full name of the component
    %price% - price of a new component
    %sell_value% - value of a component (sell value)
    %heat_transfer_rate% - heat transfer (between component or reactor) efficincy of the component
    %heat_dissipation_rate% - heat dissipation (nullifying heat) efficincy of the component
*/

/*
    %min_level%
    %selected_level%
    %max_ulocked_level%
    %max_level%
    %selected_level_bonus%
    %max_ulocked_level_bonus%
    %next_unlockable_level_bonus%
    %next_unlockable_level_cost%
*/
class StringFormatter //can be an instance because it will evenatually be optimised so the instance can be used when speed matters
{
public:
    struct Rule
    {
        std::string original;
        std::string replacement;
    };

    StringFormatter(const std::string& string);
    StringFormatter(std::string&& string);

    const std::string& originalString() const;

    std::string formattedString(const std::vector<Rule>& rules) const;
    static std::string formatString(const std::string& string, const std::vector<Rule>& rules);
protected:
    static std::string replaceOccurences(const std::string& originalStr, const std::vector<Rule>& rules);
protected:
    std::string m_original;
};

#endif // STRINGFORMATTER_H
