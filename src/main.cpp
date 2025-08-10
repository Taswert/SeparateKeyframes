#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <string>

using namespace geode::prelude;

class $modify(EditorUI) {

	CCArray* pasteObjects(gd::string p0, bool p1, bool p2) {
		auto lel = GameManager::sharedState()->getEditorLayer();
		auto skfMode = Mod::get()->getSettingValue<std::string>("skf-mode");
		
		if (skfMode == "Never") {
			//std::cout << "Keyframe Grouping Disabled, skipping Keyframe Grouping\n";
			return EditorUI::pasteObjects(p0, p1, p2);
		}

		if (skfMode == "Multi-Object" && p0.find_first_of(';') == p0.find_last_of(';')) {
			//std::cout << "Multi-Object Paste Detected, skipping Keyframe Grouping\n";
			return EditorUI::pasteObjects(p0, p1, p2);
		}
		
		std::stringstream objGroupStream(p0);
		std::string objStr;
		std::string newObjGroup;

		int iterator = 0;
		std::unordered_map<int, int> keyframeGroupMap;
		while (std::getline(objGroupStream, objStr, ';')) {
			if (objStr.empty()) continue;

			std::vector<int> commasId;
			for (int i = 0; i < objStr.size(); i++) {
				if (objStr[i] == ',') {
					commasId.push_back(i);
				}
			}

			std::string idStr = objStr.substr(commasId[0] + 1, commasId[1] - commasId[0] - 1);
			//std::cout << "Object ID: " << idStr << " / ";
			if (idStr != "3032") { // skip non-keyframe objects
				newObjGroup += objStr;
				newObjGroup += ';';
				continue; 
			}
			

			std::string kfgStr = objStr.substr(commasId[10] + 1, commasId[11] - commasId[10] - 1);
			int kfg = std::stoi(kfgStr);
			//std::cout << "KeyFrame Group: " << kfg << " / ";

			std::string kfiStr = objStr.substr(commasId[12] + 1, commasId[13] - commasId[12] - 1);
			int kfi = std::stoi(kfiStr);
			//std::cout << "KeyFrame Index: " << kfi << " / ";
			

			if (!keyframeGroupMap.contains(kfg)) {
				keyframeGroupMap[kfg] = iterator;
				iterator++;
			}
			objStr.replace(commasId[10] + 1, commasId[11] - commasId[10] - 1, std::to_string(lel->m_keyframeGroup + keyframeGroupMap[kfg]));
			commasId.clear();
			for (int i = 0; i < objStr.size(); i++) {
				if (objStr[i] == ',') {
					commasId.push_back(i);
				}
			}

			kfi--;
			objStr.replace(commasId[12] + 1, commasId[13] - commasId[12] - 1, std::to_string(kfi));

			//std::cout << "New KeyFrame Group: " << lel->m_keyframeGroup + keyframeGroupMap[kfg] << " / ";
			//std::cout << "New KeyFrame Index: " << kfi << " \n ";

			newObjGroup += objStr;
			newObjGroup += ';';
		}

		lel->m_keyframeGroup += iterator;


		auto objArr = EditorUI::pasteObjects(newObjGroup, p1, p2);
		
		//std::cout << "-- Old Obj Str --\n";
		//std::cout << p0 << "\n";
		//std::cout << "-- New Obj Str --\n";
		//std::cout << newObjGroup << "\n\n";

		lel->refreshKeyframeAnims();
		return objArr;
	}

	//void selectObject(GameObject* p0, bool p1) {
	//	if (auto kfo = dynamic_cast<KeyframeGameObject*>(p0)) {
	//		std::cout << kfo->m_keyframeGroup << " Group\n";
	//		std::cout << kfo->m_keyframeIndex << " Index\n";
	//		std::cout << GameManager::sharedState()->getEditorLayer()->m_keyframeGroup << " Next KF-Group\n";
	//		std::cout << "\n";
	//	}
	//	EditorUI::selectObject(p0, p1);
	//}
};

//$on_mod(Loaded) {
//	AllocConsole();
//	freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
//}