#pragma once

namespace ruse::reference {

  template<typename T>
  struct singleton
  {
    friend constexpr bool
    operator==(T, T)
    {
      return true;
    }

    template<typename U>
    friend constexpr bool
    operator==(T, U)
    {
      return false;
    }

    template<typename U>
    friend constexpr bool
    operator!=(T x, U y)
    {
      return !(x == y);
    }
  };

} // end of namespace ruse::reference
