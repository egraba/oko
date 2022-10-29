use clap::Parser;
use std::{thread, time};

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

    loop {
        println!("Mira!");
        thread::sleep(interval);
    }
}
