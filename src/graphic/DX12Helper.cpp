#include "DX12Helper.h"
#include <D3Dcompiler.h>
#include <filesystem>

namespace Dash
{
    bool FormatIsUAVCompatible(_In_ ID3D12Device* device, bool typedUAVLoadAdditionalFormats, DXGI_FORMAT format) noexcept
    {
        switch (format)
        {
        case DXGI_FORMAT_R32_FLOAT:
        case DXGI_FORMAT_R32_UINT:
        case DXGI_FORMAT_R32_SINT:
            // Unconditionally supported.
            return true;

        case DXGI_FORMAT_R32G32B32A32_FLOAT:
        case DXGI_FORMAT_R32G32B32A32_UINT:
        case DXGI_FORMAT_R32G32B32A32_SINT:
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
        case DXGI_FORMAT_R16G16B16A16_UINT:
        case DXGI_FORMAT_R16G16B16A16_SINT:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT_R8G8B8A8_UINT:
        case DXGI_FORMAT_R8G8B8A8_SINT:
        case DXGI_FORMAT_R16_FLOAT:
        case DXGI_FORMAT_R16_UINT:
        case DXGI_FORMAT_R16_SINT:
        case DXGI_FORMAT_R8_UNORM:
        case DXGI_FORMAT_R8_UINT:
        case DXGI_FORMAT_R8_SINT:
            // All these are supported if this optional feature is set.
            return typedUAVLoadAdditionalFormats;

        case DXGI_FORMAT_R16G16B16A16_UNORM:
        case DXGI_FORMAT_R16G16B16A16_SNORM:
        case DXGI_FORMAT_R32G32_FLOAT:
        case DXGI_FORMAT_R32G32_UINT:
        case DXGI_FORMAT_R32G32_SINT:
        case DXGI_FORMAT_R10G10B10A2_UNORM:
        case DXGI_FORMAT_R10G10B10A2_UINT:
        case DXGI_FORMAT_R11G11B10_FLOAT:
        case DXGI_FORMAT_R8G8B8A8_SNORM:
        case DXGI_FORMAT_R16G16_FLOAT:
        case DXGI_FORMAT_R16G16_UNORM:
        case DXGI_FORMAT_R16G16_UINT:
        case DXGI_FORMAT_R16G16_SNORM:
        case DXGI_FORMAT_R16G16_SINT:
        case DXGI_FORMAT_R8G8_UNORM:
        case DXGI_FORMAT_R8G8_UINT:
        case DXGI_FORMAT_R8G8_SNORM:
        case DXGI_FORMAT_R8G8_SINT:
        case DXGI_FORMAT_R16_UNORM:
        case DXGI_FORMAT_R16_SNORM:
        case DXGI_FORMAT_R8_SNORM:
        case DXGI_FORMAT_A8_UNORM:
        case DXGI_FORMAT_B5G6R5_UNORM:
        case DXGI_FORMAT_B5G5R5A1_UNORM:
        case DXGI_FORMAT_B4G4R4A4_UNORM:
            // Conditionally supported by specific devices.
            if (typedUAVLoadAdditionalFormats)
            {
                D3D12_FEATURE_DATA_FORMAT_SUPPORT formatSupport = { format, D3D12_FORMAT_SUPPORT1_NONE, D3D12_FORMAT_SUPPORT2_NONE };
                if (SUCCEEDED(device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &formatSupport, sizeof(formatSupport))))
                {
                    const DWORD mask = D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD | D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE;
                    return ((formatSupport.Support2 & mask) == mask);
                }
            }
            return false;

        default:
            return false;
        }
    }

    bool FormatIsBGR(DXGI_FORMAT format) noexcept
    {
        switch (format)
        {
        case DXGI_FORMAT_B8G8R8A8_UNORM:
        case DXGI_FORMAT_B8G8R8X8_UNORM:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
            return true;
        default:
            return false;
        }
    }

    bool FormatIsSRGB(DXGI_FORMAT format) noexcept
    {
        switch (format)
        {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
            return true;
        default:
            return false;
        }
    }

    DXGI_FORMAT ConvertSRVtoResourceFormat(DXGI_FORMAT format) noexcept
    {
        switch (format)
        {
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
        case DXGI_FORMAT_R32G32B32A32_UINT:
        case DXGI_FORMAT_R32G32B32A32_SINT:
            return DXGI_FORMAT_R32G32B32A32_TYPELESS;

        case DXGI_FORMAT_R16G16B16A16_FLOAT:
        case DXGI_FORMAT_R16G16B16A16_UNORM:
        case DXGI_FORMAT_R16G16B16A16_UINT:
        case DXGI_FORMAT_R16G16B16A16_SNORM:
        case DXGI_FORMAT_R16G16B16A16_SINT:
            return DXGI_FORMAT_R16G16B16A16_TYPELESS;

        case DXGI_FORMAT_R32G32_FLOAT:
        case DXGI_FORMAT_R32G32_UINT:
        case DXGI_FORMAT_R32G32_SINT:
            return DXGI_FORMAT_R32G32_TYPELESS;

        case DXGI_FORMAT_R10G10B10A2_UNORM:
        case DXGI_FORMAT_R10G10B10A2_UINT:
            return DXGI_FORMAT_R10G10B10A2_TYPELESS;

        case DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT_R8G8B8A8_UINT:
        case DXGI_FORMAT_R8G8B8A8_SNORM:
        case DXGI_FORMAT_R8G8B8A8_SINT:
            return DXGI_FORMAT_R8G8B8A8_TYPELESS;

        case DXGI_FORMAT_R16G16_FLOAT:
        case DXGI_FORMAT_R16G16_UNORM:
        case DXGI_FORMAT_R16G16_UINT:
        case DXGI_FORMAT_R16G16_SNORM:
        case DXGI_FORMAT_R16G16_SINT:
            return DXGI_FORMAT_R16G16_TYPELESS;

        case DXGI_FORMAT_R32_FLOAT:
        case DXGI_FORMAT_R32_UINT:
        case DXGI_FORMAT_R32_SINT:
            return DXGI_FORMAT_R32_TYPELESS;

        case DXGI_FORMAT_R8G8_UNORM:
        case DXGI_FORMAT_R8G8_UINT:
        case DXGI_FORMAT_R8G8_SNORM:
        case DXGI_FORMAT_R8G8_SINT:
            return DXGI_FORMAT_R8G8_TYPELESS;

        case DXGI_FORMAT_R16_FLOAT:
        case DXGI_FORMAT_R16_UNORM:
        case DXGI_FORMAT_R16_UINT:
        case DXGI_FORMAT_R16_SNORM:
        case DXGI_FORMAT_R16_SINT:
            return DXGI_FORMAT_R16_TYPELESS;

        case DXGI_FORMAT_R8_UNORM:
        case DXGI_FORMAT_R8_UINT:
        case DXGI_FORMAT_R8_SNORM:
        case DXGI_FORMAT_R8_SINT:
            return DXGI_FORMAT_R8_TYPELESS;

        default:
            return format;
        }
    }
    
    struct GenerateMips
    {
#pragma pack(push, 4)
        struct ConstantData
        {
            FVector2f InvOutTexelSize;
            uint32_t SrcMipIndex = 0;
        };
#pragma pack(pop)

        static const uint32_t Num32BitConstants = static_cast<uint32_t>(sizeof(ConstantData) / sizeof(uint32_t));
        static const uint32_t ThreadGroupSize = 8;

        enum RootParameterIndex
        {
            Constants,
            SourceTexture,
            TargetTexture,
            RootParameterCount
        };

        static Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateGenMipsRootSignature(
            Microsoft::WRL::ComPtr<ID3D12Device>& device)
        {
            D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
                D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
                D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

            CD3DX12_STATIC_SAMPLER_DESC sampler(
                0, // register
                D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT,
                D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
                D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
                D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

            CD3DX12_DESCRIPTOR_RANGE sourceDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
            CD3DX12_DESCRIPTOR_RANGE targetDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);

            CD3DX12_ROOT_PARAMETER rootParameters[GenerateMips::RootParameterCount] = {};
            rootParameters[GenerateMips::Constants].InitAsConstants(GenerateMips::Num32BitConstants, 0);
            rootParameters[GenerateMips::SourceTexture].InitAsDescriptorTable(1, &sourceDescriptorRange);
            rootParameters[GenerateMips::TargetTexture].InitAsDescriptorTable(1, &targetDescriptorRange);

            CD3DX12_ROOT_SIGNATURE_DESC rsigDesc;
            rsigDesc.Init(_countof(rootParameters), rootParameters, 1, &sampler, rootSignatureFlags);

            Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
            HR(CreateRootSignature(device, &rsigDesc, rootSignature));

            return rootSignature;
        }

        static Microsoft::WRL::ComPtr<ID3D12PipelineState> CreateGenMipsPipelineState(
            Microsoft::WRL::ComPtr<ID3D12Device>& device,
            Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature,
            const void* bytecode,
            size_t bytecodeSize)
        {
            D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
            desc.CS.BytecodeLength = bytecodeSize;
            desc.CS.pShaderBytecode = bytecode;
            desc.pRootSignature = rootSignature.Get();

            Microsoft::WRL::ComPtr<ID3D12PipelineState> pso;
            HR(device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pso)));

            return pso;
        }

        // Resource is UAV compatible
        static void GenerateMips_UnorderedAccessPath(Microsoft::WRL::ComPtr<ID3D12Device>& device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
            Microsoft::WRL::ComPtr<ID3D12Resource>& resource, Microsoft::WRL::ComPtr<ID3D12PipelineState>& pso, Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature)
        {
            const auto desc = resource->GetDesc();
            ASSERT(!FormatIsBGR(desc.Format) && !FormatIsSRGB(desc.Format));

            CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);

            D3D12_COMMAND_LIST_TYPE commandListType = commandList->GetType();

            ASSERT(commandListType != D3D12_COMMAND_LIST_TYPE_COPY);
            const D3D12_RESOURCE_STATES originalState = (commandListType == D3D12_COMMAND_LIST_TYPE_COMPUTE)
                ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

            // Create a staging resource if we have to
            Microsoft::WRL::ComPtr<ID3D12Resource> staging;
            if ((desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS) == 0)
            {
                D3D12_RESOURCE_DESC stagingDesc = desc;
                stagingDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
                stagingDesc.Format = ConvertSRVtoResourceFormat(desc.Format);

                ThrowIfFailed(device->CreateCommittedResource(
                    &defaultHeapProperties,
                    D3D12_HEAP_FLAG_NONE,
                    &stagingDesc,
                    D3D12_RESOURCE_STATE_COPY_DEST,
                    nullptr,
                    IID_PPV_ARGS(staging.GetAddressOf())));

                // Copy the top mip of resource to staging
                //TransitionResource(commandList.Get(), resource, originalState, D3D12_RESOURCE_STATE_COPY_SOURCE);

                commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(), originalState, D3D12_RESOURCE_STATE_COPY_SOURCE));

                CD3DX12_TEXTURE_COPY_LOCATION src(resource.Get(), 0);
                CD3DX12_TEXTURE_COPY_LOCATION dst(staging.Get(), 0);
                commandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

                //TransitionResource(commandList.Get(), staging.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

                commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(staging.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));
            }
            else
            {
                // Resource is already a UAV so we can do this in-place
                staging = resource;

                //TransitionResource(commandList.Get(), staging.Get(), originalState, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
                commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(staging.Get(), originalState, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE));
            }

            // Create a descriptor heap that holds our resource descriptors
            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;
            D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
            descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            descriptorHeapDesc.NumDescriptors = desc.MipLevels;
            device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(descriptorHeap.GetAddressOf()));

            auto descriptorSize = static_cast<int>(device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

            // Create SRV for all mips
            CD3DX12_CPU_DESCRIPTOR_HANDLE handleIt(descriptorHeap->GetCPUDescriptorHandleForHeapStart());
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels = desc.MipLevels;

            device->CreateShaderResourceView(staging.Get(), &srvDesc, handleIt);

            // Create the UAVs for the tail
            for (uint16_t mip = 1; mip < desc.MipLevels; ++mip)
            {
                D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
                uavDesc.Format = desc.Format;
                uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
                uavDesc.Texture2D.MipSlice = mip;

                handleIt.Offset(descriptorSize);
                device->CreateUnorderedAccessView(staging.Get(), nullptr, &uavDesc, handleIt);
            }

            // Set up UAV barrier (used in loop)
            D3D12_RESOURCE_BARRIER barrierUAV = {};
            barrierUAV.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
            barrierUAV.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            barrierUAV.UAV.pResource = staging.Get();

            // Barrier for transitioning the subresources to UAVs
            D3D12_RESOURCE_BARRIER srv2uavDesc = {};
            srv2uavDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            srv2uavDesc.Transition.pResource = staging.Get();
            srv2uavDesc.Transition.Subresource = 0;
            srv2uavDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
            srv2uavDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;

            // Barrier for transitioning the subresources to SRVs
            D3D12_RESOURCE_BARRIER uav2srvDesc = {};
            uav2srvDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            uav2srvDesc.Transition.pResource = staging.Get();
            uav2srvDesc.Transition.Subresource = 0;
            uav2srvDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
            uav2srvDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;

            // based on format, select srgb or not
            //Microsoft::WRL::ComPtr<ID3D12PipelineState> pso = mGenMipsResources->generateMipsPSO;

            // Set up state
            commandList->SetComputeRootSignature(rootSignature.Get());
            commandList->SetPipelineState(pso.Get());
            commandList->SetDescriptorHeaps(1, descriptorHeap.GetAddressOf());
            commandList->SetComputeRootDescriptorTable(GenerateMips::SourceTexture, descriptorHeap->GetGPUDescriptorHandleForHeapStart());

            // Get the descriptor handle -- uavH will increment over each loop
            CD3DX12_GPU_DESCRIPTOR_HANDLE uavH(
                descriptorHeap->GetGPUDescriptorHandleForHeapStart(),
                descriptorSize); // offset by 1 descriptor

            // Process each mip
            auto mipWidth = static_cast<uint32_t>(desc.Width);
            uint32_t mipHeight = desc.Height;
            for (uint32_t mip = 1; mip < desc.MipLevels; ++mip)
            {
                mipWidth = std::max<uint32_t>(1, mipWidth >> 1);
                mipHeight = std::max<uint32_t>(1, mipHeight >> 1);

                // Transition the mip to a UAV
                srv2uavDesc.Transition.Subresource = mip;
                commandList->ResourceBarrier(1, &srv2uavDesc);

                // Bind the mip subresources
                commandList->SetComputeRootDescriptorTable(GenerateMips::TargetTexture, uavH);

                // Set constants
                GenerateMips::ConstantData constants;
                constants.SrcMipIndex = mip - 1;
                constants.InvOutTexelSize = FVector2f(1 / float(mipWidth), 1 / float(mipHeight));
                commandList->SetComputeRoot32BitConstants(
                    GenerateMips::Constants,
                    GenerateMips::Num32BitConstants,
                    &constants,
                    0);

                // Process this mip
                // Every thread process one pixel ( ThreadGroupSize == 8 )
                commandList->Dispatch(
                    (mipWidth + GenerateMips::ThreadGroupSize - 1) / GenerateMips::ThreadGroupSize,
                    (mipHeight + GenerateMips::ThreadGroupSize - 1) / GenerateMips::ThreadGroupSize,
                    1);

                //Transition uav resource to flag none
                commandList->ResourceBarrier(1, &barrierUAV);

                // Transition the mip to an SRV
                uav2srvDesc.Transition.Subresource = mip;
                commandList->ResourceBarrier(1, &uav2srvDesc);

                // Offset the descriptor heap handles
                uavH.Offset(descriptorSize);
            }

            // If the staging resource is NOT the same as the resource, we need to copy everything back
            if (staging.Get() != resource.Get())
            {
                // Transition the resources ready for copy
                D3D12_RESOURCE_BARRIER barrier[2] = {};
                barrier[0].Type = barrier[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                barrier[0].Transition.Subresource = barrier[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                barrier[0].Transition.pResource = staging.Get();
                barrier[0].Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
                barrier[0].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;

                barrier[1].Transition.pResource = resource.Get();
                barrier[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
                barrier[1].Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;

                commandList->ResourceBarrier(2, barrier);

                // Copy the entire resource back
                commandList->CopyResource(resource.Get(), staging.Get());

                // Transition the target resource back to pixel shader resource
                //TransitionResource(commandList.Get(), resource, D3D12_RESOURCE_STATE_COPY_DEST, originalState);
                commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, originalState));
            }
            else
            {
                //TransitionResource(commandList.Get(), staging.Get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, originalState);
                commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(staging.Get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, originalState));
            }
        }
    };

	void GenerateMipmap(Microsoft::WRL::ComPtr<ID3D12Device>& device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,
        Microsoft::WRL::ComPtr<ID3D12Resource>& resource)
	{
        if (resource == nullptr)
        {
            throw std::invalid_argument("Nullptr passed to GenerateMips");
        }

        D3D12_COMMAND_LIST_TYPE commandListType = commandList->GetType();

        ASSERT(commandListType != D3D12_COMMAND_LIST_TYPE_COPY);

        const D3D12_RESOURCE_DESC desc = resource->GetDesc();

        if (desc.MipLevels == 1)
        {
            // Nothing to do 
            return;
        }
        if (desc.MipLevels == 0)
        {
            throw std::exception("GenerateMips: texture has no mips");
        }
        if (desc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D)
        {
            throw std::exception("GenerateMips only supports Texture2D resources");
        }
        if (desc.DepthOrArraySize != 1)
        {
            throw std::exception("GenerateMips only supports 2D textures of array size 1");
        }

        Microsoft::WRL::ComPtr<ID3DBlob> generateMipsShader;

        UINT shaderCompileFlag = 0;

#ifdef _DEBUG
        shaderCompileFlag |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // _DEBUG

        shaderCompileFlag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

        std::filesystem::path currentPath = std::filesystem::current_path();
        std::filesystem::path shaderPath = currentPath / "src\\resources\\generateMips.hlsl";

        HR(D3DCompileFromFile(shaderPath.c_str(), nullptr, nullptr, "CSMain", "cs_5_0", shaderCompileFlag, 0, &generateMipsShader, nullptr));

        Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = GenerateMips::CreateGenMipsRootSignature(device);
        Microsoft::WRL::ComPtr<ID3D12PipelineState> generateMipsPSO = GenerateMips::CreateGenMipsPipelineState(device, rootSignature, generateMipsShader->GetBufferPointer(), generateMipsShader->GetBufferSize());

        bool mTypedUAVLoadAdditionalFormats = false;
        bool mStandardSwizzle64KBSupported = false;

        D3D12_FEATURE_DATA_D3D12_OPTIONS options = {};
        if (SUCCEEDED(device->CheckFeatureSupport(
            D3D12_FEATURE_D3D12_OPTIONS,
            &options,
            sizeof(options))))
        {
            mTypedUAVLoadAdditionalFormats = options.TypedUAVLoadAdditionalFormats != 0;
            mStandardSwizzle64KBSupported = options.StandardSwizzle64KBSupported != 0;
        }

        bool uavCompat = FormatIsUAVCompatible(device.Get(), mTypedUAVLoadAdditionalFormats, desc.Format);

        if (!uavCompat && !FormatIsSRGB(desc.Format) && !FormatIsBGR(desc.Format))
        {
            throw std::exception("GenerateMips doesn't support this texture format on this device");
        }
        
        if (uavCompat)
        {
            GenerateMips::GenerateMips_UnorderedAccessPath(device, commandList, resource, generateMipsPSO, rootSignature);
        }
	}
    
}