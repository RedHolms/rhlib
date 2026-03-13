#include <rh/log/Logger.hpp>

using rh::log::Logger;

Logger::Logger(Config& config) {
  auto outputs = config.StealOutputs();
  auto formatters = config.StealFormatters();
  auto rules = config.StealRules();

  for (auto& [id, output] : outputs)
    m_outputs.emplace(id, std::move(output));

  for (auto& [id, formatter] : formatters)
    m_formatters.emplace(id, std::move(formatter));

  m_rules.reserve(rules.size());
  for (auto& ruleConfig : rules) {
    auto targetLevels = ruleConfig.StealTargetLevels();
    auto outputId = ruleConfig.StealOutputId();
    auto formatterId = ruleConfig.StealFormatterId();

    if (!m_outputs.contains(outputId) || !m_formatters.contains(formatterId))
      continue;

    m_rules.emplace_back(targetLevels, outputId, formatterId);
  }
}

Logger::~Logger() = default;

void Logger::Log(Message const& message) const {
  // FIXME bad
  for (auto const& rule : m_rules) {
    if (!DoMessageSatisfyRule(message, rule))
      continue;

    // Constructor guarantees that rule's outputId and formatterId is right
    auto& output = m_outputs.at(rule.outputId);
    auto& formatter = m_formatters.at(rule.formatterId);

    String text;
    {
      std::lock_guard _lock { formatter.mutex };
      text = formatter.formatter->Format(message, output.hasColors);
    }
    {
      std::lock_guard _lock { output.mutex };
      output.output->Print(text);
    }
  }
}

bool Logger::DoMessageSatisfyRule(Message const& message, Rule const& rule) const {
  return rule.DoesApplyToLevel(message.level);
}
