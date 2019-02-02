#include "ComponentFactory.h"
#include "Component.h"
#include "Cell.h"
#include "HeatVent.h"
#include "Capacitor.h"
#include "NeutronReflector.h"
#include "CoolingCell.h"
#include "ReactorPlating.h"
#include "HeatExchanger.h"
#include "HeatInlet.h"
#include "HeatOutlet.h"

ComponentFactory::ComponentFactory(const std::string& filePath)
{
    Configuration config(filePath);
    loadCellsFromConfiguration(config);
    loadHeatVentsFromConfiguration(config);
    loadCapacitorsFromConfiguration(config);
    loadNeutronReflectorsFromConfiguration(config);
    loadCoolingCellsFromConfiguration(config);
    loadReactorPlatingsFromConfiguration(config);
    loadHeatExchangersFromConfiguration(config);
    loadHeatInletsFromConfiguration(config);
    loadHeatOutletsFromConfiguration(config);
}
const std::shared_ptr<Component>& ComponentFactory::component(const std::string& name) const
{
    return m_components.at(name);
}
const std::shared_ptr<Component::CommonData>& ComponentFactory::commonData(const std::string& name) const
{
    return m_commonDatas.at(name);
}
const std::string& ComponentFactory::onShopDisplayDescription(const std::string& name) const
{
    return commonData(name)->onShopDisplayDescription();
}
const std::string& ComponentFactory::onGridDescription(const std::string& name) const
{
    return commonData(name)->onGridDescription();
}
std::shared_ptr<Component> ComponentFactory::createComponent(const std::string& name) const
{
    return component(name)->clone();
}
std::shared_ptr<Component::CommonData>& ComponentFactory::registerComponentCommonData(const std::string& name, const std::shared_ptr<Component::CommonData>& commonData)
{
    return m_commonDatas[name] = commonData;
}
std::shared_ptr<Component>& ComponentFactory::registerComponent(const std::string& name, const std::shared_ptr<Component>& component)
{
    return m_components[name] = component;
}

template <class T>
void ComponentFactory::loadComponentFromConfiguration(ConfigurationNode& config)
{
    auto component = std::make_shared<T>(config);
    auto commonData = component->commonData();
    const auto& name = commonData->name();
    registerComponentCommonData(name, commonData);
    registerComponent(name, component);

    std::cout << "Loaded component: " << name << '\n';
}

template <class T>
void ComponentFactory::loadAllComponentsFromConfiguration(ConfigurationNode& config)
{
    int numberOfComponents = config.length();
    for(int i = 1; i <= numberOfComponents; ++i)
    {
        ConfigurationNode node = config[i];
        loadComponentFromConfiguration<T>(node);
    }
}

void ComponentFactory::loadCellsFromConfiguration(Configuration& config)
{
    ConfigurationNode node = config["cells"];
    loadAllComponentsFromConfiguration<Cell>(node);
}

void ComponentFactory::loadHeatVentsFromConfiguration(Configuration& config)
{
    ConfigurationNode node = config["heatVents"];
    loadAllComponentsFromConfiguration<HeatVent>(node);
}

void ComponentFactory::loadCapacitorsFromConfiguration(Configuration& config)
{
    ConfigurationNode node = config["capacitors"];
    loadAllComponentsFromConfiguration<Capacitor>(node);
}

void ComponentFactory::loadNeutronReflectorsFromConfiguration(Configuration& config)
{

    ConfigurationNode node = config["neutronReflectors"];
    loadAllComponentsFromConfiguration<NeutronReflector>(node);
}

void ComponentFactory::loadCoolingCellsFromConfiguration(Configuration& config)
{
    ConfigurationNode node = config["coolingCells"];
    loadAllComponentsFromConfiguration<CoolingCell>(node);
}

void ComponentFactory::loadReactorPlatingsFromConfiguration(Configuration& config)
{
    ConfigurationNode node = config["reactorPlatings"];
    loadAllComponentsFromConfiguration<ReactorPlating>(node);
}

void ComponentFactory::loadHeatExchangersFromConfiguration(Configuration& config)
{
    ConfigurationNode node = config["heatExchangers"];
    loadAllComponentsFromConfiguration<HeatExchanger>(node);
}

void ComponentFactory::loadHeatInletsFromConfiguration(Configuration& config)
{
    ConfigurationNode node = config["heatInlets"];
    loadAllComponentsFromConfiguration<HeatInlet>(node);
}

void ComponentFactory::loadHeatOutletsFromConfiguration(Configuration& config)
{
    ConfigurationNode node = config["heatOutlets"];
    loadAllComponentsFromConfiguration<HeatOutlet>(node);
}
void ComponentFactory::updateBonusesInComponentCommonDatas(UpgradePool& upgradePool)
{
    for(auto& commonDataPair : m_commonDatas)
    {
        auto& commonData = commonDataPair.second;
        commonData->updateBonuses(upgradePool);
    }
}
void ComponentFactory::resetTemplateComponentsToTheirBaseState()
{
    for(auto& componentPair : m_components)
    {
        auto& component = componentPair.second;
        component->resetToBaseState();
    }
}
