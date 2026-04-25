# Contributing to Tetris / 贡献指南

Thank you for your interest in contributing to this project!

## How to Contribute / 如何贡献

### Bug Reports / Bug 报告

1. Check existing issues before creating new ones
2. Include your operating system and compiler version
3. Provide minimal reproduction steps
4. Include expected vs actual behavior

### Code Contributions / 代码贡献

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Coding Standards / 编码规范

- Follow the existing code style (bilingual comments in Chinese/English)
- Add unit tests for new features
- Update documentation when changing functionality
- Keep commits atomic and descriptive

## Testing / 测试

```bash
# Build and run tests
cd build && cmake .. && make tetris_test && ./tetris_test

# Run with coverage
./tetris_test --use-colour no
```

## Questions? / 问题？

Feel free to open an issue for any questions about the codebase.
