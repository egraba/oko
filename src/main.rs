use clap::Parser;
use cursive::views::{LinearLayout, TextView};
use std::time;
use sysinfo::{System, SystemExt};

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
    let _interval = time::Duration::from_secs(args.interval);

    let sys = System::new_all();

    let mut siv = cursive::default();

    siv.add_global_callback('q', |s| s.quit());

    let linear_layout = LinearLayout::vertical()
        .child(TextView::new("oko <o> - Press 'q' to exit."))
        .child(TextView::new(format!(
            "os name: {:?}, release: {:?}",
            sys.name(),
            sys.os_version()
        )));

    siv.add_layer(linear_layout);

    siv.run();
}
