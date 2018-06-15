#include "pch.h"
#include <cstdint>

#include <d3d11_4.h>
#include <dxgi1_5.h>
#include <wrl/client.h>

#include <DirectXMath.h>

#include <winrt/base.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.UI.ViewManagement.h>

#include <triangle_vertex.h>
#include <triangle_vertex_instanced.h>
#include <triangle_pixel.h>

#include <triangle_constants.h>


using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace Microsoft::WRL;


static uint32_t align16(uint32_t v)
{
    return (v + 15) & (~15);
}

static uint32_t align256(uint32_t v)
{
    return (v + 255) & (~255);
}

struct exception : public std::exception
{
    exception(HRESULT h) : m_h(h)
    {

    }

    HRESULT m_h;
};

inline void ThrowIfFailed(HRESULT hr)
{
    if (hr != S_OK)
    {
        throw exception(hr);
    }
}

static ComPtr<ID3D11Device5> CreateDevice()
{
    ComPtr<ID3D11Device> r;
    D3D_FEATURE_LEVEL levels[]{ D3D_FEATURE_LEVEL_11_0 };
    ThrowIfFailed(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, &levels[0], 1, D3D11_SDK_VERSION, r.GetAddressOf(), nullptr, nullptr));

    ComPtr<ID3D11Device5> r0;
    ThrowIfFailed(r.As(&r0));
    return r0;
}

static ComPtr<ID3D11DeviceContext4> CreateImmediateContext(ID3D11Device* d )
{
    ComPtr<ID3D11DeviceContext> r;
    d->GetImmediateContext(r.GetAddressOf());

    ComPtr<ID3D11DeviceContext4> r0;
    ThrowIfFailed(r.As(&r0));

    return r0;
}

static ComPtr<IDXGISwapChain1> CreateSwapChain(const CoreWindow& w, ID3D11Device* d)
{
    ComPtr<IDXGIFactory2> f;
    ComPtr<IDXGISwapChain1> r;
    
    ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG,__uuidof(IDXGIFactory2), reinterpret_cast<void**>(f.GetAddressOf())));

    DXGI_SWAP_CHAIN_DESC1 desc = {};

    desc.BufferCount    = 3;
    desc.Format         = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.Width          = static_cast<UINT>(w.Bounds().Width);
    desc.Height         = static_cast<UINT>(w.Bounds().Height);
    desc.SampleDesc.Count = 1;
    desc.SwapEffect     = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.BufferUsage    = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.AlphaMode      = DXGI_ALPHA_MODE_IGNORE;
    desc.Scaling        = DXGI_SCALING_NONE;

    ThrowIfFailed(f->CreateSwapChainForCoreWindow(d, winrt::get_abi(w), &desc, nullptr, r.GetAddressOf()));

    return r;
}

static ComPtr<ID3D11RenderTargetView1> CreateSwapChainView(IDXGISwapChain1* swap_chain, ID3D11Device5* device)
{
    ComPtr<ID3D11Texture2D> texture;

    ThrowIfFailed(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(texture.GetAddressOf())));
    CD3D11_RENDER_TARGET_VIEW_DESC desc(texture.Get(), D3D11_RTV_DIMENSION_TEXTURE2D);
    ComPtr<ID3D11RenderTargetView1> r;
    ThrowIfFailed(device->CreateRenderTargetView1(texture.Get(), nullptr, r.GetAddressOf()));
    return r;
}

static ComPtr<ID3D11VertexShader> CreateTriangleVertexShader(ID3D11Device5* device)
{
    ComPtr<ID3D11VertexShader> r;
    ThrowIfFailed(device->CreateVertexShader(g_triangle_vertex, sizeof(g_triangle_vertex), nullptr, r.GetAddressOf()));
    return r;
}

static ComPtr<ID3D11VertexShader> CreateTriangleVertexInstancedShader(ID3D11Device5* device)
{
    ComPtr<ID3D11VertexShader> r;
    ThrowIfFailed(device->CreateVertexShader(g_triangle_vertex_instanced, sizeof(g_triangle_vertex_instanced), nullptr, r.GetAddressOf()));
    return r;
}

static ComPtr<ID3D11PixelShader> CreateTrianglePixelShader(ID3D11Device5* device)
{
    ComPtr<ID3D11PixelShader> r;
    ThrowIfFailed(device->CreatePixelShader(g_triangle_pixel, sizeof(g_triangle_pixel), nullptr, r.GetAddressOf()));
    return r;
}

static ComPtr<ID3D11RasterizerState2> CreateRasterizerState(ID3D11Device5* device)
{
    ComPtr<ID3D11RasterizerState2> r;

    D3D11_RASTERIZER_DESC2 state = {};

    state.FillMode              = D3D11_FILL_SOLID;
    state.CullMode              = D3D11_CULL_BACK;
    state.FrontCounterClockwise = TRUE;
    state.DepthClipEnable       = TRUE;
    state.ScissorEnable         = TRUE;

    ThrowIfFailed(device->CreateRasterizerState2(&state, r.GetAddressOf()));
    return r;
}

static ComPtr<ID3D11BlendState1> CreateBlendState(ID3D11Device5* device)
{
    ComPtr<ID3D11BlendState1> r;

    D3D11_BLEND_DESC1 state = {};
    state.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    ThrowIfFailed(device->CreateBlendState1(&state, r.GetAddressOf()));
    return r;
}

static ComPtr<ID3D11DepthStencilState> CreateDepthStencilState(ID3D11Device5* device)
{
    ComPtr<ID3D11DepthStencilState> r;

    D3D11_DEPTH_STENCIL_DESC state = {};
    state.DepthEnable = TRUE;
    state.DepthFunc = D3D11_COMPARISON_LESS;
    ThrowIfFailed(device->CreateDepthStencilState(&state, r.GetAddressOf()));
    return r;
}

static ComPtr<ID3D11Buffer> CreateFrameConstantsBuffer(ID3D11Device5* device, uint32_t size)
{
    ComPtr<ID3D11Buffer> r;

    D3D11_BUFFER_DESC state     = {};
    state.ByteWidth             = align256(size);
    state.Usage                 = D3D11_USAGE_DYNAMIC;
    state.BindFlags             = D3D11_BIND_CONSTANT_BUFFER;
    state.CPUAccessFlags        = D3D11_CPU_ACCESS_WRITE;
    state.MiscFlags             = 0;
    state.StructureByteStride   = 0;

    
    ThrowIfFailed(device->CreateBuffer(&state, nullptr, r.GetAddressOf()));

    return r;
}

class ViewProvider : public winrt::implements<ViewProvider, IFrameworkView, IFrameworkViewSource>
{
    public:

    IFrameworkView CreateView();
    void Initialize(const CoreApplicationView& v);
    void Uninitialize();
    void Run();

    void Load(winrt::hstring h);
    void SetWindow(const CoreWindow& w);
    void OnWindowClosed(const CoreWindow&w, const CoreWindowEventArgs& a);
    void OnActivated(const CoreApplicationView&, const IActivatedEventArgs&);
    void OnWindowSizeChanged(const CoreWindow& w, const WindowSizeChangedEventArgs& a);

    bool m_window_running = true;
    CoreWindow::Closed_revoker                  m_closed;
    CoreWindow::SizeChanged_revoker             m_size_changed;
    CoreApplicationView::Activated_revoker      m_activated;
    
    ComPtr<ID3D11Device5>                       m_device;
    ComPtr<ID3D11DeviceContext4>                m_device_context;
    ComPtr<IDXGISwapChain1>                     m_swap_chain;

    ComPtr<ID3D11VertexShader>                  m_triangle_vertex;
    ComPtr<ID3D11VertexShader>                  m_triangle_vertex_instanced;
    ComPtr<ID3D11PixelShader>                   m_triangle_pixel;
    ComPtr<ID3D11RasterizerState2>              m_rasterizer_state;
    ComPtr<ID3D11BlendState1>                   m_blend_state;
    ComPtr<ID3D11DepthStencilState>             m_depth_stencil_state;

    std::array<ComPtr<ID3D11Buffer>, 3 >        m_frame_constants;
    std::array< std::vector< uint8_t>, 3>       m_frame_constants_shadow;

    uint32_t                                    m_back_buffer_width = 0;
    uint32_t                                    m_back_buffer_height = 0;
    uint32_t                                    m_frame_counter = 0;
    uint32_t                                    m_frame_pointer = 0;

    const uint32_t                              m_frame_constants_size = 128 * 1024;

    void                                        UploadFrameConstants(ID3D11DeviceContext* ctx, uint32_t frame_counter);
    void*                                       AllocateFrameConstant(uint32_t size);
    uint32_t                                    FrameConstantOffset(const void* a);
    uint32_t                                    FrameConstantSize(size_t s);
};

void* ViewProvider::AllocateFrameConstant(uint32_t size)
{
    void* r = &m_frame_constants_shadow[m_frame_counter][m_frame_pointer];
    m_frame_pointer += align256(size);
    return r;
}

uint32_t ViewProvider::FrameConstantOffset(const void* a)
{
    auto e = reinterpret_cast<const uint8_t*>(a);
    auto b = reinterpret_cast<const uint8_t*>(&m_frame_constants_shadow[m_frame_counter][0]);

    return static_cast<int32_t>(e - b) / 16;
}

uint32_t ViewProvider::FrameConstantSize(size_t s)
{
    return (align256(static_cast<uint32_t>(s)) / 16);
}

IFrameworkView ViewProvider::CreateView()
{
    return *this;
}

void ViewProvider::Initialize(const CoreApplicationView& v)
{
    m_activated = v.Activated(winrt::auto_revoke, { this, &ViewProvider::OnActivated });
    m_device = CreateDevice();
    m_device_context = CreateImmediateContext(m_device.Get());
}

void ViewProvider::Uninitialize()
{

}

void ViewProvider::Run()
{
    while (m_window_running)
    {
        CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

        {
            ComPtr<ID3D11RenderTargetView1> m_swap_chain_view = CreateSwapChainView(m_swap_chain.Get(), m_device.Get());

            m_device_context->ClearState();

            {
                float clear_value[4] = { 0.2f, 0.2f, 0.2f, 0.0f };
                m_device_context->ClearRenderTargetView(m_swap_chain_view.Get(), clear_value);
            }
          

            {
                ID3D11RenderTargetView* views[1] = { m_swap_chain_view.Get() };
                m_device_context->OMSetRenderTargets(1, views, nullptr);
                m_device_context->OMSetDepthStencilState(m_depth_stencil_state.Get(), 0);
            }

            {
                m_device_context->OMSetBlendState(m_blend_state.Get(), nullptr, 0xFFFFFFFF);
            }

            {
                m_device_context->RSSetState(m_rasterizer_state.Get());

                D3D11_RECT r        = { 0, 0, m_back_buffer_width, m_back_buffer_height };

                m_device_context->RSSetScissorRects(1, &r);

                D3D11_VIEWPORT v    = {};
                v.TopLeftX          = 0;
                v.TopLeftY          = 0;
                v.MinDepth          = 0.0f;
                v.MaxDepth          = 1.0f;
                v.Width             = static_cast<float>(m_back_buffer_width);
                v.Height            = static_cast<float>(m_back_buffer_height);

                m_device_context->RSSetViewports(1, &v);
            }

            {
                m_device_context->VSSetShader(m_triangle_vertex.Get(), nullptr, 0);
            }
            {
                m_device_context->PSSetShader(m_triangle_pixel.Get(), nullptr, 0);
            }
            {
                m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
            }

            {
                const int subdivision_count     = 40 + 1;
                const int vertical_segments     = subdivision_count;
                const int horizontal_segments   = subdivision_count * 2;
                const int vertex_count          = (vertical_segments + 1) * (horizontal_segments + 1);
                const int index_count           = vertex_count * 3;

                frame_constants*  v0                            = reinterpret_cast<frame_constants*> (AllocateFrameConstant(sizeof(frame_constants)));
                sphere_constants* v1                            = reinterpret_cast<sphere_constants*> (AllocateFrameConstant(sizeof(sphere_constants)));
                object_constants* v2                            = reinterpret_cast<object_constants*> (AllocateFrameConstant(sizeof(object_constants)));
                spheres*          v3                            = reinterpret_cast<spheres*> (AllocateFrameConstant(sizeof(spheres)));

                const float PI                                  = 3.14159265358979323846264338327950288;
                float aspect_ratio                              = static_cast<float>(m_back_buffer_width) / static_cast<float>(m_back_buffer_height);
                float fov                                       = (2.0f * PI ) / 3.0f;

                DirectX::XMVECTOR               camera_position = DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
                DirectX::XMVECTOR               camera_look_at  = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
                DirectX::XMVECTOR               camera_up       = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
                DirectX::XMMATRIX perspective                   = DirectX::XMMatrixPerspectiveFovLH(fov, aspect_ratio, 0.1f, 100.0f);
                DirectX::XMMATRIX view                          = DirectX::XMMatrixLookAtLH(camera_position, camera_look_at, camera_up);

                DirectX::XMVECTOR               ball_position   = DirectX::XMVectorSet( 0.0f, 0.0f, 0.0f, 1 );
                DirectX::XMMATRIX               world           = DirectX::XMMatrixTranslation(0, 0, 0);

                //constant buffers
                DirectX::XMStoreFloat4x4A(&v0->m_perspective, DirectX::XMMatrixTranspose(perspective));
                DirectX::XMStoreFloat4x4A(&v0->m_view, DirectX::XMMatrixTranspose(view));
                DirectX::XMVECTOR   sphere                      = DirectX::XMVectorSet(1.0f, static_cast<float>(subdivision_count), 0.0f, 0.0f);

                //constant buffers
                DirectX::XMStoreFloat4A(&v1->m_sphere_parameters, sphere);
                DirectX::XMStoreFloat4x4A(&v2->m_world, DirectX::XMMatrixTranspose(world));

                UploadFrameConstants(m_device_context.Get(), m_frame_counter);

                {
                    ID3D11Buffer* buffers[] =
                    {
                        m_frame_constants[m_frame_counter].Get(),
                        m_frame_constants[m_frame_counter].Get(),
                        m_frame_constants[m_frame_counter].Get()
                    };

                    uint32_t offsets[] =
                    {
                        FrameConstantOffset(v0),
                        FrameConstantOffset(v1),
                        FrameConstantOffset(v2),
                    };

                    uint32_t counts[] =
                    {
                        FrameConstantSize(sizeof(frame_constants)),
                        FrameConstantSize(sizeof(sphere_constants)),
                        FrameConstantSize(sizeof(object_constants)),
                    };

                    m_device_context->VSSetConstantBuffers1(0, 3, buffers, &offsets[0], &counts[0]);
                    m_device_context->Draw(index_count, 0);
                }

                {
                    m_device_context->VSSetShader(m_triangle_vertex_instanced.Get(), nullptr, 0);
                    
                    float x[3]      = { -10.2f, 0.2f,   0.1f };
                    float y[3]      = { 3.f,    5.2f,   0.2f };
                    float z[3]      = { 5.f,    12.2f,  0.5f };
                    float radius[3] = { 0.2f,   0.6f,   0.3f };

                    for (auto i = 0U; i < 3; ++i)
                    {
                        DirectX::XMVECTOR   sphere = DirectX::XMVectorSet(x[i], y[i], z[i], radius[i]);
                        DirectX::XMStoreFloat4A(&v3->m_spheres[i], sphere);
                    }

                    ID3D11Buffer* buffers[] =
                    {
                        m_frame_constants[m_frame_counter].Get(),
                        m_frame_constants[m_frame_counter].Get(),
                        m_frame_constants[m_frame_counter].Get()
                    };

                    uint32_t offsets[] =
                    {
                        FrameConstantOffset(v0),
                        FrameConstantOffset(v1),
                        FrameConstantOffset(v3),
                    };

                    uint32_t counts[] =
                    {
                        FrameConstantSize(sizeof(frame_constants)),
                        FrameConstantSize(sizeof(sphere_constants)),
                        FrameConstantSize(sizeof(spheres)),
                    };

                    m_device_context->VSSetConstantBuffers1(0, 3, buffers, &offsets[0], &counts[0]);
                    m_device_context->DrawInstanced(index_count, 3, 0, 0);
                }
            }
        }

        m_swap_chain->Present(0, 0);

        m_frame_counter = m_frame_counter + 1;
        m_frame_counter %= 3;
        m_frame_pointer = 0;
    }
}

void ViewProvider::Load(winrt::hstring h)
{
    m_triangle_vertex = CreateTriangleVertexShader(m_device.Get());
    m_triangle_pixel = CreateTrianglePixelShader(m_device.Get());
    m_triangle_vertex_instanced = CreateTriangleVertexInstancedShader(m_device.Get());

    m_blend_state = CreateBlendState(m_device.Get());
    m_rasterizer_state = CreateRasterizerState(m_device.Get());
    m_depth_stencil_state = CreateDepthStencilState(m_device.Get());

    m_frame_constants[0] = CreateFrameConstantsBuffer(m_device.Get(), m_frame_constants_size);
    m_frame_constants[1] = CreateFrameConstantsBuffer(m_device.Get(), m_frame_constants_size);
    m_frame_constants[2] = CreateFrameConstantsBuffer(m_device.Get(), m_frame_constants_size);

    m_frame_constants_shadow[0] = std::vector<uint8_t>(m_frame_constants_size);
    m_frame_constants_shadow[1] = std::vector<uint8_t>(m_frame_constants_size);
    m_frame_constants_shadow[2] = std::vector<uint8_t>(m_frame_constants_size);

    m_frame_counter = 0;
}

void ViewProvider::SetWindow(const CoreWindow& w)
{
    m_closed = w.Closed(winrt::auto_revoke, { this, &ViewProvider::OnWindowClosed });
    m_size_changed = w.SizeChanged(winrt::auto_revoke, { this, &ViewProvider::OnWindowSizeChanged });

    m_swap_chain = CreateSwapChain(w, m_device.Get());

    m_back_buffer_width = static_cast<UINT>(w.Bounds().Width);
    m_back_buffer_height = static_cast<UINT>(w.Bounds().Height);
}

void ViewProvider::OnWindowClosed(const CoreWindow&w, const CoreWindowEventArgs& a)
{
    m_window_running = false;
}

void ViewProvider::OnActivated(const CoreApplicationView&, const IActivatedEventArgs&)
{
    CoreWindow::GetForCurrentThread().Activate();
}

void ViewProvider::OnWindowSizeChanged(const CoreWindow& w, const WindowSizeChangedEventArgs& a)
{
    ThrowIfFailed(m_swap_chain->ResizeBuffers(3, static_cast<UINT>(a.Size().Width), static_cast<UINT>(a.Size().Height), DXGI_FORMAT_R8G8B8A8_UNORM, 0));
    m_back_buffer_width     = static_cast<UINT>(a.Size().Width);
    m_back_buffer_height    = static_cast<UINT>(a.Size().Height);
}

void ViewProvider::UploadFrameConstants(ID3D11DeviceContext* ctx, uint32_t frame_counter)
{
    auto&& d = m_frame_constants[frame_counter].Get();
    auto&& s = &m_frame_constants_shadow[frame_counter][0];

    D3D11_MAPPED_SUBRESOURCE mapped = {};

    ThrowIfFailed( ctx->Map( d, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped) );
    memcpy_s(mapped.pData, m_frame_constants_size, s, m_frame_constants_size);
    ctx->Unmap(d,0);
}
namespace computational_geometry
{
    void test2();
}

int32_t __stdcall wWinMain( HINSTANCE, HINSTANCE,PWSTR, int32_t )
{
    computational_geometry::test2();
    ThrowIfFailed(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
    CoreApplication::Run(ViewProvider());
    CoUninitialize();
    return 0;
}
