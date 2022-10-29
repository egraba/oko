use clap::Parser;
use std::{thread, time};
use sysinfo::{NetworkExt, ProcessExt, System, SystemExt};

/// Agent collecting information about the OS on which it is installed
#[derive(Parser)]
#[command(version)]
struct Args {
    /// Sets refresh interval (in seconds)
    #[arg(short, long, default_value_t = 2)]
    interval: u64,
}

fn main() {
    let args = Args::parse();
    let interval = time::Duration::from_secs(args.interval);

    let mut sys = System::new_all();

    loop {
        sys.refresh_all();

        println!("=> disks:");
        for disk in sys.disks() {
            println!("{:?}", disk);
        }

        println!("=> networks:");
        for (interface_name, data) in sys.networks() {
            println!("{}: {}/{} B", interface_name, data.received(), data.transmitted());
        }

        println!("=> components:");
        for component in sys.components() {
            println!("{:?}", component);
        }

        println!("=> system:");
        println!("total memory: {} bytes", sys.total_memory());
        println!("used memory : {} bytes", sys.used_memory());
        println!("total swap  : {} bytes", sys.total_swap());
        println!("used swap   : {} bytes", sys.used_swap());

        println!("System name:             {:?}", sys.name());
        println!("System kernel version:   {:?}", sys.kernel_version());
        println!("System OS version:       {:?}", sys.os_version());
        println!("System host name:        {:?}", sys.host_name());

        println!("NB CPUs: {}", sys.cpus().len());

        for (pid, process) in sys.processes() {
            println!("[{}] {} {:?}", pid, process.name(), process.disk_usage());
        }

        thread::sleep(interval);
    }
}
