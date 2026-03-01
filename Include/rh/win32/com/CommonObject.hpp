#pragma once

#include <rh/win32/com/CO.hpp>
#include <tuple>
#include <Unknwn.h>

template <class... Interfaces>
class CommonObject : public Interfaces... {
  IMMOVABLE_CLASS(CommonObject);

  static_assert(sizeof...(Interfaces) > 0, "At least one interface required");

protected:
  CommonObject() = default;

public:
  virtual ~CommonObject() = default;

public: // IUnknown
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** outObject) override {
    if (!outObject)
      return E_POINTER;

    *outObject = nullptr;

    if (iid == __uuidof(IUnknown)) {
      *outObject = GetCanonicalIUnknown();
    }
    else {
      QueryInterfaceInternal(iid, outObject);
    }

    if (*outObject) {
      AddRef();
      return S_OK;
    }

    return E_NOINTERFACE;
  }

  ULONG STDMETHODCALLTYPE AddRef() override {
    return static_cast<ULONG>(InterlockedIncrement(&m_refCount));
  }

  ULONG STDMETHODCALLTYPE Release() override {
    ULONG count = static_cast<ULONG>(InterlockedDecrement(&m_refCount));

    if (count == 0)
      delete this;

    return count;
  }

private:
  IUnknown* GetCanonicalIUnknown() noexcept {
    using FirstInterface = std::tuple_element_t<0, std::tuple<Interfaces...>>;
    return implicit_cast<IUnknown*>(implicit_cast<FirstInterface*>(this));
  }

  void QueryInterfaceInternal(REFIID iid, void** outObject) noexcept {
    ((iid == __uuidof(Interfaces) ? (*outObject = implicit_cast<Interfaces*>(this), true)
                                  : false) ||
     ...);
  }

private:
  volatile LONG m_refCount = 1;
};

#define DECLARE_COMMON_OBJECT_MAKER(CLASS)                                                         \
public:                                                                                            \
  template <typename... ArgsT>                                                                     \
  static CO<CLASS> Create(ArgsT&&... args) {                                                       \
    return CO(new CLASS(std::forward<ArgsT>(args)...), InheritReference);                          \
  }
