#include "Scene.h"
using namespace PinEngine;
using namespace PinEngine::UI;

//#include "Scene.h"
//
//namespace PinEngine
//{
//	void Scene::AddObject(std::shared_ptr<RenderableEngineObject2D> object)
//	{
//		//Make sure parent has child added to its list of children if the object's parent is set
//		if (object->parent != nullptr)
//		{
//			if (!object->parent->HasChild(object))
//			{
//				object->parent->AddChild(object);
//			}
//		}
//		objects_2d.push_back(object);
//	}
//
//	bool Scene::HasObject(std::shared_ptr<RenderableEngineObject2D> object)
//	{
//		for (size_t i = 0; i < objects_2d.size(); i++)
//		{
//			if (objects_2d[i] == object)
//			{
//				return true;
//			}
//		}
//		return false;
//	}
//
//	void Scene::RemoveObject(std::shared_ptr<RenderableEngineObject2D> object)
//	{
//		for (size_t i=0; i<objects_2d.size(); i++)
//		{
//			if (objects_2d[i] == object)
//			{
//				objects_2d.erase(objects_2d.begin() + i);
//				return;
//			}
//		}
//	}
//	std::vector<std::shared_ptr<RenderableEngineObject2D>> Scene::Get2DObjects()
//	{
//		return objects_2d;
//	}
//}

void Scene::AddWidget(std::shared_ptr<Widget> widget)
{
	assert(widget->widgetType != WidgetType::Unknown, "Unknown or uninitialized widget added to scene.");
	for (auto w : widgets)
	{
		if (widget == w)
			return;
	}

	//Make sure parent has child added to its list of children if the object's parent is set
	/*if (widget->parent != nullptr)
	{
		if (!widget->parent->HasChild(widget))
		{
			widget->parent->AddChild(widget);
		}
	}*/
	widgets.push_back(widget);
	/*for (auto w : widget->children)
	{
		AddWidget(w);
	}*/
}

bool Scene::HasWidget(std::shared_ptr<Widget> widget)
{
	for (size_t i = 0; i < widgets.size(); i++)
	{
		if (widgets[i] == widget)
		{
			return true;
		}
	}
	return false;
}

void Scene::RemoveWidget(std::shared_ptr<Widget> widget)
{
	for (size_t i = 0; i < widgets.size(); i++)
	{
		if (widgets[i] == widget)
		{
			widgets.erase(widgets.begin() + i);
			return;
		}
	}
}

std::vector<std::shared_ptr<Widget>> Scene::GetWidgets()
{
	return widgets;
}