use crate::display::launch_display_mode;
use clap::{arg, command};
use std::time;

pub mod display;

fn main() {
    let matches = command!()
        .about("Agent collecting information about the OS on which it is installed")
        .arg(arg!(-i --interval <INTERVAL> "Sets refresh interval (in seconds)"))
        .arg(arg!(-d --display "Displays the output on the terminal"))
        .get_matches();

    let interval = matches
        .get_one::<String>("interval")
        .expect("Default interval is 2")
        .parse::<u64>()
        .unwrap();
    let interval = time::Duration::from_secs(interval).as_secs();

    launch_display_mode(interval);
}
