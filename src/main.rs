use clap::Parser;

/// Agent collecting information about the OS on which it is installed
#[derive(Parser)]
#[command(version)]
struct Args {
    /// Sets refresh interval (in seconds)
    #[arg(short, long, default_value_t = 2)]
    interval: u8,
}

fn main() {
    let _args = Args::parse();
}
