use cursive::views::{LinearLayout, Panel, TextView};
use std::time;
use sysinfo::{System, SystemExt};

pub fn launch_display_mode(interval: time::Duration) {
    let sys = System::new_all();

    let mut siv = cursive::default();
    siv.load_toml(include_str!("theme.toml")).unwrap();

    const MAX_REFRESH_RATE: u64 = 30;
    let interval = interval.as_secs();
    match interval {
        0..=MAX_REFRESH_RATE => siv.set_fps(interval.try_into().unwrap()),
        _ => siv.set_fps(MAX_REFRESH_RATE.try_into().unwrap()),
    }

    siv.add_global_callback('q', |s| s.quit());

    let linear_layout = LinearLayout::vertical()
        .child(TextView::new("oko <o> - Press 'q' to exit."))
        .child(
            Panel::new(TextView::new(format!(
                "os name: {:?}, release: {:?}",
                sys.name(),
                sys.os_version()
            )))
            .title("Machine"),
        )
        .child(
            Panel::new(TextView::new(format!(
                "memory used: {:?}, free: {:?}, swapused: {:?}, swapfree: {:?}",
                sys.used_memory(),
                sys.free_memory(),
                sys.used_swap(),
                sys.free_swap()
            )))
            .title("Usage"),
        );

    siv.add_layer(linear_layout);

    siv.run();
}
