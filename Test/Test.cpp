#include <iostream>
#include <memory>

int globalId = 0;

class GameObject {
private:
	int id = 0;
	GameObject* m_pParent = nullptr;
	std::shared_ptr<GameObject> m_pChild = nullptr;
	std::shared_ptr<GameObject> m_pSibling = nullptr;
public:
	GameObject() { id = globalId++; 
		std::cout << id << " - Created" << std::endl;
	};
	~GameObject() {
		std::cout << id << " - Deleted" << std::endl;
	}

	void SetParent(GameObject* target) { m_pParent = target; }
	void SetChild(std::shared_ptr<GameObject> target) { m_pChild = target; }
	void SetSibling(std::shared_ptr<GameObject> target) { m_pSibling = target; }
};

int main()
{
	std::unique_ptr<GameObject> mainObject1 = std::make_unique<GameObject>();
	std::unique_ptr<GameObject> mainObject2 = std::make_unique<GameObject>();

	std::shared_ptr<GameObject> mainObject = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> childObject = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> siblingObject = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> childObject_1 = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> siblingObject_1 = std::make_shared<GameObject>();


	mainObject->SetChild(childObject);

	childObject->SetParent(mainObject.get());
	childObject->SetChild(childObject_1);
	childObject->SetSibling(siblingObject);

	childObject_1->SetParent(childObject.get());
	childObject_1->SetSibling(siblingObject_1);

	mainObject1->SetChild(mainObject);
	mainObject2->SetChild(mainObject);
}