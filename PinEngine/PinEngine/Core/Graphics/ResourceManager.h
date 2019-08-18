#pragma once
#include <d3d11.h>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include "Shaders.h"
#include <wrl/client.h>
#include "PipelineState.h"

namespace PinEngine
{
	struct PipelineResourceMaps
	{
		//Maps that use shared ptr
		#define PIPELINE_RESOURCE_SHAREDPTR_OBJECT(datatype, mapname) \
		std::unordered_map<std::wstring, std::shared_ptr<datatype>> mapname; 

		//Maps that use comptr
		#define PIPELINE_RESOURCE_COMPTR_OBJECT(datatype, mapname) \
		std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<datatype>> mapname;

		//Include inline to generate the shared_ptr and comptr maps
		#include "ResourceManager.inl"
	};

	class ResourceManager
	{
	public:
		#pragma region
			//Shared ptr/comptr register resource declarations
			#define PIPELINE_RESOURCE_SHAREDPTR_OBJECT(datatype, mapname) \
			static void RegisterResource(std::wstring id, std::shared_ptr<datatype> resource);
			#define PIPELINE_RESOURCE_COMPTR_OBJECT(datatype, mapname) \
			static void RegisterResource(std::wstring id, Microsoft::WRL::ComPtr<datatype> resource);
			#include "ResourceManager.inl"

			//Shared ptr/comptr get resource declarations
			#define PIPELINE_RESOURCE_SHAREDPTR_OBJECT(datatype, mapname) \
			static bool GetResource(std::wstring id, std::shared_ptr<datatype> &pOutResource);
			#define PIPELINE_RESOURCE_COMPTR_OBJECT(datatype, mapname) \
			static bool GetResource(std::wstring id, Microsoft::WRL::ComPtr<datatype> &pOutResource);
			#include "ResourceManager.inl"
		#pragma endregion This region generates the declarations for the comptr/sharedptr map object functions GetResource and RegisterResource

	private:
		static PipelineResourceMaps resourceMaps;
	};
}