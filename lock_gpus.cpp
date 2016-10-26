#include <iostream>
#include <thread>
#include <chrono>
#include <vexcl/devlist.hpp>

#include <boost/program_options.hpp>

int main(int argc, char *argv[]) {
    namespace po = boost::program_options;

    po::options_description desc("Options");

    desc.add_options()
        ("help,h", "Show this help.")
        ("gpus,n",
         po::value<int>()->default_value(1),
         "Number of GPUs to lock. "
        )
        (
         "wait,w",
         po::bool_switch()->default_value(false),
         "Wait until termination."
        )
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    int ngpus = vm["gpus"].as<int>();

    vex::Context ctx(vex::Filter::Exclusive(vex::Filter::Env && vex::Filter::Count(ngpus)));

    if (ctx.size() == ngpus) {
        std::cout << "Locked GPU(s):\n" << ctx << std::endl;
    } else {
        std::cout << "Failed to lock GPU(s)" << std::endl;
        return 1;
    }

    if (vm["wait"].as<bool>()) {
        while(true) std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
