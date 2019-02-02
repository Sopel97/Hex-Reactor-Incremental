#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <map>
#include <string>
#include <memory>

#include "Configuration.h"
#include "Component.h"

class ComponentFactory
{
public:
    ComponentFactory(const std::string& filePath);
    const std::shared_ptr<Component>& component(const std::string& name) const;
    const std::shared_ptr<Component::CommonData>& commonData(const std::string& name) const;
    const std::string& onShopDisplayDescription(const std::string& name) const;
    const std::string& onGridDescription(const std::string& name) const;
    std::shared_ptr<Component> createComponent(const std::string& name) const;
    std::shared_ptr<Component::CommonData>& registerComponentCommonData(const std::string& name, const std::shared_ptr<Component::CommonData>& commonData);
    std::shared_ptr<Component>& registerComponent(const std::string& name, const std::shared_ptr<Component>& component);

    template <class T>
    void loadComponentFromConfiguration(ConfigurationNode& config);
    template <class T>
    void loadAllComponentsFromConfiguration(ConfigurationNode& config);

    void loadCellsFromConfiguration(Configuration& config);
    void loadHeatVentsFromConfiguration(Configuration& config);
    void loadCapacitorsFromConfiguration(Configuration& config);
    void loadNeutronReflectorsFromConfiguration(Configuration& config);
    void loadCoolingCellsFromConfiguration(Configuration& config);
    void loadReactorPlatingsFromConfiguration(Configuration& config);
    void loadHeatExchangersFromConfiguration(Configuration& config);
    void loadHeatInletsFromConfiguration(Configuration& config);
    void loadHeatOutletsFromConfiguration(Configuration& config);

    void updateBonusesInComponentCommonDatas(UpgradePool& upgradePool);
    void resetTemplateComponentsToTheirBaseState();
private:
    std::map<std::string, std::shared_ptr<Component>> m_components; //<name, component>
    std::map<std::string, std::shared_ptr<Component::CommonData>> m_commonDatas; //<name, static component data>
};

#endif // COMPONENTFACTORY_H
