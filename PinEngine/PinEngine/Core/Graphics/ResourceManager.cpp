#include "ResourceManager.h"

namespace PinEngine
{
#define PIPELINE_RESOURCE_SHAREDPTR_OBJECT(datatype, mapname) \
void ResourceManager::RegisterResource(std::wstring id, std::shared_ptr<datatype> resource) \
{ \
	resourceMaps.mapname[id] = resource; \
}
#define PIPELINE_RESOURCE_COMPTR_OBJECT(datatype, mapname) \
void ResourceManager::RegisterResource(std::wstring id, Microsoft::WRL::ComPtr<datatype> resource) \
{ \
	resourceMaps.mapname[id] = resource; \
}
#include "ResourceManager.inl"

	//SharedPtr/Comptr GetResource Definitions
#define PIPELINE_RESOURCE_SHAREDPTR_OBJECT(datatype, mapname) \
bool ResourceManager::GetResource(std::wstring id, std::shared_ptr<datatype> & pOutResource) \
{ \
	std::unordered_map<std::wstring, std::shared_ptr<datatype>>::iterator it; \
	it = resourceMaps.mapname.find(id); \
 \
	if (it != resourceMaps.mapname.end()) /*If resource was found*/ \
	{ \
		pOutResource = it->second; \
		return true; /*return true if resource was found*/ \
	} \
\
	pOutResource = nullptr; \
	return false; /*return false if no matching resource found*/ \
}
#define PIPELINE_RESOURCE_COMPTR_OBJECT(datatype, mapname) \
bool ResourceManager::GetResource(std::wstring id, Microsoft::WRL::ComPtr<datatype> & pOutResource) \
{ \
	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<datatype>>::iterator it; \
	it = resourceMaps.mapname.find(id); \
 \
	if (it != resourceMaps.mapname.end()) /*If resource was found*/ \
	{ \
		pOutResource = it->second; \
		return true; /*return true if resource was found*/ \
	} \
\
	pOutResource = nullptr; \
	return false; /*return false if no matching resource found*/ \
}
#include "ResourceManager.inl"
#pragma endregion This is a region to generate the definitions for the GetResource and RegisterResource functions for the data types from the PipelineManager.inl inline file

	bool ResourceManager::UnregisterResourceID3D11RenderTargetView(std::wstring id)
	{
		std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11RenderTargetView>>::iterator it;
		it = resourceMaps.renderTargetViews.find(id); 
			
		if (it != resourceMaps.renderTargetViews.end()) /*If resource was found*/
		{ 
			resourceMaps.renderTargetViews.erase(it);
			return true; /*return true if resource was found*/ 
		} 
		return false; /*return false if no matching resource found*/ 
	}

	PipelineResourceMaps ResourceManager::resourceMaps;
	ResourceManager rm;
	
	//Initialize an instance of a resource manager to call static constructor
}
