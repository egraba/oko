use clap::Parser;
use cursive::views::{LinearLayout, Panel, TextView};
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
    let interval = time::Duration::from_secs(args.interval);

    let sys = System::new_all();

    let mut siv = cursive::default();
    siv.load_toml(include_str!("theme.toml")).unwrap();

    const MAX_REFRESH_RATE: u64 = 30;
    match interval.as_secs() {
        0..=MAX_REFRESH_RATE => siv.set_fps(interval.as_secs().try_into().unwrap()),
        _ => siv.set_fps(MAX_REFRESH_RATE.try_into().unwrap()),
    }

    siv.add_global_callback('q', |s| s.quit());

    let linear_layout = LinearLayout::vertical()
        .child(TextView::new("oko <o> - Press 'q' to exit."))
        .child(Panel::new(TextView::new(format!(
            "os name: {:?}, release: {:?}",
            sys.name(),
            sys.os_version()
        ))))
        .child(Panel::new(TextView::new(format!(
            "memory used: {:?}, free: {:?}, swapused: {:?}, swapfree: {:?}",
            sys.used_memory(),
            sys.free_memory(),
            sys.used_swap(),
            sys.free_swap()
        ))));

    siv.add_layer(linear_layout);

    siv.run();
}
