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

    // static constexpr const std::string s_1 = "Hello ";

    // static constexpr const std::string s_2 = "world!";

    // static constexpr const std::string s_3 = s_1 + s_2;

    // std::cout << s_3 << std::endl;

    wiringPi<WPI_MODE_PHYS()> Pi;

    // while (Pi.run())
    // {
    //     ;
    // }

    const gpio_t i = Pi.digitalReadOnboard<3>();

    std::cout << "i = " << i << std::endl;

    // for (size_t i = 0; i < 64; i++)
    // for (pinTable::const_iterator i = Pi.pinMap().begin(); i < Pi.pinMap().end(); i++)
    // {
    //     // std::cout << "i = " << *i << ", pin = " << Pi.pinMap()[i] << std::endl;
    //     std::cout << "i = " << *i << std::endl;
    // }

    // std::cout << "i = " << Pi.digitalReadOnboard<3>() << std::endl;

    // struct sigaction sa
    // {
    //     signalHandler,
    // };
    // memset(&sa, 0, sizeof(sa));

    // const struct sigaction sa = initialiseSigaction();

    // std::cout << "sa.sa_mask = " << *sa.sa_mask.__val << std::endl;
    // std::cout << "sa.sa_flags = " << sa.sa_flags << std::endl;

    return 0;
}