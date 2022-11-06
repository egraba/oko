use crate::oko::collect_machine_info;
use crate::oko::{CpuInfo, HardwareInfo, MemoryInfo, NetworkInfo, OsInfo};
use crossterm::execute;
use crossterm::terminal::{
    disable_raw_mode, enable_raw_mode, EnterAlternateScreen, LeaveAlternateScreen,
};
use std::io;
use std::thread;
use std::time;
use tui::{
    backend::CrosstermBackend,
    layout::Rect,
    text::{Span, Spans},
    widgets::{Block, Borders, Paragraph},
    Terminal,
};

pub fn launch_display_mode(_interval: time::Duration) -> Result<(), io::Error> {
    enable_raw_mode()?;
    let mut stdout = io::stdout();
    execute!(stdout, EnterAlternateScreen)?;
    let backend = CrosstermBackend::new(stdout);
    let mut terminal = Terminal::new(backend)?;

    terminal.clear()?;

    terminal.draw(|f| {
        let size = f.size();

        let instruction = "oko <o> - Press 'q' to exit.";
        let instruction = Paragraph::new(instruction);
        let instruction_size = Rect {
            x: 0,
            y: 0,
            width: size.width,
            height: 1,
        };
        f.render_widget(instruction, instruction_size);

        let hwi = HardwareInfo::new();
        let mut neti = NetworkInfo::new();
        let mut cpui = CpuInfo::new();
        let mut osi = OsInfo::new();
        let mut memi = MemoryInfo::new();
        collect_machine_info(&mut neti, &mut cpui, &mut memi, &mut osi);

        let machine = vec![
            Spans::from(vec![Span::raw(format!(
                "hardware serialnumber: {}, type: {}, model: {}",
                hwi.serialnumber(),
                hwi.hwtype(),
                hwi.model(),
            ))]),
            Spans::from(vec![Span::raw(format!(
                "network hostname: {}, ip: {}, macaddress: {}",
                neti.hostname(),
                neti.ip(),
                neti.macaddress(),
            ))]),
            Spans::from(vec![Span::raw(format!(
                "cpu arch: {}, model: {}, ncpus: {}",
                cpui.arch(),
                cpui.model(),
                cpui.ncpus(),
            ))]),
            Spans::from(vec![Span::raw(format!(
                "memory physmem: {}, swaptotal: {}",
                memi.physmem(),
                memi.swaptotal(),
            ))]),
            Spans::from(vec![Span::raw(format!(
                "os name: {}, release: {}",
                osi.name(),
                osi.release(),
            ))]),
        ];
        let machine =
            Paragraph::new(machine).block(Block::default().title("Machine").borders(Borders::ALL));
        let machine_size = Rect {
            x: 0,
            y: 1,
            width: size.width,
            height: 7,
        };
        f.render_widget(machine, machine_size);
    })?;

    thread::sleep(time::Duration::from_secs(5));

    disable_raw_mode()?;
    execute!(terminal.backend_mut(), LeaveAlternateScreen)?;
    terminal.show_cursor()?;

    Ok(())
}
