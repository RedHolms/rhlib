#pragma once

#include <concepts>
#include <filesystem>
#include <map>
#include <rh/log/formatting.hpp>
#include <rh/log/output.hpp>
#include <string>

namespace rh::log {

class RuleConfig final {
  friend class Config;
  friend class Logger;

private:
  constexpr explicit RuleConfig(Config* parent) : m_parent(parent) {}

public:
  /**
   * Make rule apply to any level
   */
  forceinline RuleConfig& ForAnyLevel() {
    m_targetLevels = -1;
    return *this;
  }

  /**
   * Make rule apply to specified level
   */
  forceinline RuleConfig& ForLevel(Level level) {
    m_targetLevels |= 1 << (size_t)level;
    return *this;
  }

  /**
   * Make rule apply to specified level and any level higher
   */
  forceinline RuleConfig& ForLevelAndHigher(Level level) {
    m_targetLevels |= ~((1 << (size_t)level) - 1);
    return *this;
  }

  /**
   * Make rule apply to specified level and any level lower
   */
  forceinline RuleConfig& ForLevelAndLower(Level level) {
    m_targetLevels |= (1 << ((size_t)level + 1)) - 1;
    return *this;
  }

  // TODO: More conditions

  /**
   * Set output to use when rule is satisfied
   */
  forceinline RuleConfig& Output(std::string_view outputId) {
    m_outputId = outputId;
    return *this;
  }

  /**
   * Set formatter to use when rule is satisfied
   */
  forceinline RuleConfig& Formatter(std::string_view formatterId) {
    m_formatterId = formatterId;
    return *this;
  }

  /**
   * End rule config and return to the main logger config
   */
  forceinline Config& End() {
    return *m_parent;
  }

private:
  LevelsBitSet StealTargetLevels() noexcept {
    return std::move(m_targetLevels);
  }

  std::string&& StealOutputId() noexcept {
    return std::move(m_outputId);
  }

  std::string&& StealFormatterId() noexcept {
    return std::move(m_formatterId);
  }

private:
  Config* m_parent;
  LevelsBitSet m_targetLevels = 0;
  std::string m_outputId;
  std::string m_formatterId;
};

class Config final {
  friend class Logger;

public:
  forceinline Config() = default;

  // Because we store maps to unique pointers
  Config(Config const&) = delete;
  Config& operator=(Config const&) = delete;

public:
  /**
   * Register new output
   */
  forceinline Config& AddOutput(std::string_view id, std::unique_ptr<Output>&& output) {
    m_outputs.emplace(std::string(id), std::move(output));
    return *this;
  }

  /**
   * Register new output. Template version
   */
  template <std::derived_from<Output> T, typename... ArgsT>
  forceinline Config& AddOutput(std::string_view id, ArgsT&&... args) {
    m_outputs.emplace(std::string(id), std::make_unique<T>(std::forward<ArgsT>(args)...));
    return *this;
  }

  /**
   * Register new console (stdout) output
   */
  forceinline Config& AddStdOutOutput(std::string_view id) {
    return AddOutput<ConsoleOutput>(id);
  }

  /**
   * Register new console (stderr) output
   */
  forceinline Config& AddStdErrOutput(std::string_view id) {
    return AddOutput<ConsoleErrorOutput>(id);
  }

  /**
   * Register new file output using fmt::output_file
   * TODO: maybe remove fmtlib dependency?
   */
  forceinline Config& AddFileOutput(
    std::string_view id,
    std::fs::path const& filePath,
    int oflag = fmt::file::WRONLY | fmt::file::CREATE | fmt::file::TRUNC
  ) {
    auto output = std::make_unique<FmtStreamOutput>(fmt::output_file(filePath.string(), oflag));
    m_outputs.emplace(std::string(id), std::move(output));
    return *this;
  }

  /**
   * Register new formatter
   */
  forceinline Config& AddFormatter(std::string_view id, std::unique_ptr<Formatter>&& formatter) {
    m_formatters.emplace(std::string(id), std::move(formatter));
    return *this;
  }

  /**
   * Register new formatter. Template version
   */
  template <std::derived_from<Formatter> T, typename... ArgsT>
  forceinline Config& AddFormatter(std::string_view id, ArgsT&&... args) {
    m_formatters.emplace(std::string(id), std::make_unique<T>(std::forward<ArgsT>(args)...));
    return *this;
  }

  /**
   * Begin new logging rule
   */
  forceinline RuleConfig& Rule() {
    m_rules.push_back(RuleConfig(this));
    return *(m_rules.end() - 1);
  }

private:
  std::map<std::string, std::unique_ptr<Output>>&& StealOutputs() noexcept {
    return std::move(m_outputs);
  }

  std::map<std::string, std::unique_ptr<Formatter>>&& StealFormatters() noexcept {
    return std::move(m_formatters);
  }

  std::vector<RuleConfig>&& StealRules() noexcept {
    return std::move(m_rules);
  }

private:
  std::map<std::string, std::unique_ptr<Output>> m_outputs;
  std::map<std::string, std::unique_ptr<Formatter>> m_formatters;
  std::vector<RuleConfig> m_rules;
};

} // namespace rh::log
