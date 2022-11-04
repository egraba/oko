use sysinfo::{System, SystemExt};

#[derive(Debug)]
pub struct Os {
    pub name: String,
    pub release: String,
}

const NOT_RETRIEVED: &str = "NOT_RETRIEVED";

pub fn collect_machine_info(os: &mut Os) {
    let mut sys = System::new_all();
    sys.refresh_all();

    os.name = sys.name().expect(NOT_RETRIEVED);
    os.release = sys.os_version().expect(NOT_RETRIEVED);
}
