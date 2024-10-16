#include "wiringPi.H"

using namespace WiringPi;

int main(void)
{

    // static constexpr auto a1 = constexpr_strcat(
    //     std::to_array("Hello"),
    //     std::to_array(" "),
    //     std::to_array("world"),
    //     std::to_array("!"));

    // constexpr const std::string_view s_1 = toStringView(a1);

    // std::cout << "s_1 = " << s_1 << std::endl;

    wiringPi<WPI_MODE_GPIO()> Pi;

    // const gpio_t i = Pi.digitalReadOnboard<3>();

    // std::cout << "i = " << i << std::endl;

    // for (size_t i = 0; i < 64; i++)
    for (pinTable::const_iterator i = Pi.pinMap().begin(); i < Pi.pinMap().end(); i++)
    {
        // std::cout << "i = " << *i << ", pin = " << Pi.pinMap()[i] << std::endl;
        std::cout << "i = " << *i << std::endl;
    }

    return 0;
}