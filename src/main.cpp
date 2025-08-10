#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <string>

using namespace geode::prelude;

class $modify(EditorUI) {

	CCArray* pasteObjects(gd::string p0, bool p1, bool p2) {
		auto lel = GameManager::sharedState()->getEditorLayer();
		auto skfMode = Mod::get()->getSettingValue<std::string>("skf-mode");

		if (skfMode == "Never") {
			std::cout << "Keyframe Grouping Disabled, skipping Keyframe Grouping\n";
			return EditorUI::pasteObjects(p0, p1, p2);
		}
		
		if (skfMode == "Multi-Object" && static_cast<std::string>(p0).find_first_of(';') == static_cast<std::string>(p0).find_last_of(';')) {
			std::cout << "Multi-Object Paste Detected, skipping Keyframe Grouping\n";
			return EditorUI::pasteObjects(p0, p1, p2);
		}


		int iterator = 0;
		std::unordered_map<int, int> keyframeGroupMap;

		lel->m_keyframeGroup += iterator;

		auto objArr = EditorUI::pasteObjects(p0, p1, p2);
		std::cout << "Count: " << objArr->count() << "\n";

		for (auto obj : CCArrayExt<GameObject*>(objArr)) {
			if (auto kfo = dynamic_cast<KeyframeGameObject*>(obj)) {
				auto kfg = kfo->m_keyframeGroup;
				auto kfi = kfo->m_keyframeIndex;

				std::cout << "KeyFrame Group: " << kfg << " / ";
				std::cout << "KeyFrame Index: " << kfi << " / ";

				if (!keyframeGroupMap.contains(kfg)) {
					keyframeGroupMap[kfg] = iterator;
					iterator++;
				}

				kfo->m_keyframeGroup = lel->m_keyframeGroup + keyframeGroupMap[kfg];

				std::cout << "New KeyFrame Group: " << kfo->m_keyframeGroup << " / ";
				std::cout << "New KeyFrame Index: " << kfi << "\n";
			}

		}

		lel->refreshKeyframeAnims();
		return objArr;
	}

	void selectObject(GameObject* p0, bool p1) {
		if (auto kfo = dynamic_cast<KeyframeGameObject*>(p0)) {
			std::cout << kfo->m_keyframeGroup << " Group\n";
			std::cout << kfo->m_keyframeIndex << " Index\n";
			std::cout << GameManager::sharedState()->getEditorLayer()->m_keyframeGroup << " Next KF-Group\n";
			std::cout << "\n";
		}
		EditorUI::selectObject(p0, p1);
	}
};

//$on_mod(Loaded) {
//	AllocConsole();
//	freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
//}