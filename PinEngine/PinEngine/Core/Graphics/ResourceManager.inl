//PIPELINE RESOURCE OBJECTS THAT USE A SHAREDPTR EXAMPLE: Custom classes like PixelShader, VertexShader, PipelineState
#ifndef PIPELINE_RESOURCE_SHAREDPTR_OBJECT
#define PIPELINE_RESOURCE_SHAREDPTR_OBJECT(datatype, mapname)
#endif

PIPELINE_RESOURCE_SHAREDPTR_OBJECT(PixelShader, pixelshaders)
PIPELINE_RESOURCE_SHAREDPTR_OBJECT(VertexShader, vertexshaders)
PIPELINE_RESOURCE_SHAREDPTR_OBJECT(PipelineState, pipelinestates)
PIPELINE_RESOURCE_SHAREDPTR_OBJECT(VertexBuffer<Vertex_2D_Texture>, vertices_2d_texture)
PIPELINE_RESOURCE_SHAREDPTR_OBJECT(Texture, textures)


#undef PIPELINE_RESOURCE_SHAREDPTR_OBJECT

//PIPELINE RESOURCES THAT USE COMPTRS EXAMPLE: RASTERIZER STATE, DEPTH STENCIL STATE, BLEND STATE
#ifndef PIPELINE_RESOURCE_COMPTR_OBJECT
#define PIPELINE_RESOURCE_COMPTR_OBJECT(datatype, mapname)
#endif

PIPELINE_RESOURCE_COMPTR_OBJECT(ID3D11RasterizerState, rasterizerStates)
PIPELINE_RESOURCE_COMPTR_OBJECT(ID3D11BlendState, blendStates)
PIPELINE_RESOURCE_COMPTR_OBJECT(ID3D11DepthStencilState, depthStencilStates)

#undef PIPELINE_RESOURCE_COMPTR_OBJECT