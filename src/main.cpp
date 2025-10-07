#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <string>

using namespace geode::prelude;

class $modify(EditorUI) {

	CCArray* pasteObjects(gd::string p0, bool p1, bool p2) {
		auto lel = GameManager::sharedState()->getEditorLayer();
		auto skfMode = Mod::get()->getSettingValue<std::string>("skf-mode");

		if (skfMode == "Never") {
			return EditorUI::pasteObjects(p0, p1, p2);
		}
		
		if (skfMode == "Multi-Object" && std::string_view(p0).find_first_of(';') == std::string_view(p0).find_last_of(';')) {
			return EditorUI::pasteObjects(p0, p1, p2);
		}


		int iterator = 0;
		std::unordered_map<int, int> keyframeGroupMap;

		auto objArr = EditorUI::pasteObjects(p0, p1, p2);

		for (auto obj : CCArrayExt<GameObject*>(objArr)) {
			if (auto kfo = typeinfo_cast<KeyframeGameObject*>(obj)) {
				auto kfg = kfo->m_keyframeGroup;
				auto kfi = kfo->m_keyframeIndex;

				if (!keyframeGroupMap.contains(kfg)) {
					keyframeGroupMap[kfg] = iterator;
					iterator++;
				}

				kfo->m_keyframeGroup = lel->m_keyframeGroup + keyframeGroupMap[kfg];
			}

		}

		lel->refreshKeyframeAnims();
		return objArr;
	}
};