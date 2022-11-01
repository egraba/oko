use crate::display::launch_display_mode;
use crate::log::launch_log_mode;
use clap::{arg, command, value_parser};
use std::time;

pub mod display;
pub mod log;

fn main() {
    let matches = command!()
        .about("Agent collecting information about the OS on which it is installed")
        .arg(
            arg!(-i --interval [INTERVAL] "Sets refresh interval (in seconds)")
                .value_parser(value_parser!(u64))
                .default_value("2"),
        )
        .arg(
            arg!(<MODE>)
                .help("What mode to run the program in")
                .value_parser(["display", "log"]),
        )
        .get_matches();

    let interval = matches
        .get_one::<u64>("interval")
        .expect("Default interval is 2");
    let interval = time::Duration::from_secs(*interval).as_secs();

    match matches
        .get_one::<String>("MODE")
        .expect("'MODE' is required and parsing will fail if its missing")
        .as_str()
    {
        "display" => {
            launch_display_mode(interval);
        }
        "log" => {
            launch_log_mode(interval);
        }
        _ => unreachable!(),
    }
}
