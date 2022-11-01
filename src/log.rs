use log::info;
use std::{thread, time};

pub fn launch_log_mode(interval: time::Duration) {
    tracing_subscriber::fmt().json().init();

    info!("Machine info");

    loop {
        info!("Usage info");
        thread::sleep(interval);
    }
}
