(deffunction ask-question (?question ?yes-node ?no-node)
  (printout t ?question " (yes/no): ")
  (bind ?answer (read))
  (if (eq ?answer yes)
    then (assert (next-node ?yes-node))
    else (assert (next-node ?no-node))))

(defrule main-start
  (not (next-node ?))
  =>
  (assert (next-node q1)))

(defrule q1
  (next-node q1)
  =>
  (ask-question "Реакция возникает в течение 1–6 часов после контакта с аллергеном?" q2_yes q2_no))

(defrule q2_yes
  (next-node q2_yes)
  =>
  (ask-question "В анамнезе есть указания на перенесённую тяжёлую системную реакцию (анафилаксию)?" q3_anaph q3_resp))

(defrule q3_anaph
  (next-node q3_anaph)
  =>
  (ask-question "Реакцию вызвало жалящее насекомое?" leaf_bee q4_anaph_no))

(defrule leaf_bee
  (next-node leaf_bee)
  =>
  (printout t "Диагноз: Аллергия на яд перепончатокрылых (ос, пчёл)" crlf)
  (printout t "--- Диагностика завершена ---" crlf)
  (halt))

(defrule q4_anaph_no
  (next-node q4_anaph_no)
  =>
  (ask-question "Реакция возникла во время или после внутривенного введения лекарства?" leaf_anaph_drug q5_anaph_food))

(defrule leaf_anaph_drug
  (next-node leaf_anaph_drug)
  =>
  (printout t "Диагноз: Анафилаксия на ЛС (например, НПВП/антибиотики)" crlf)
  (halt))

(defrule q5_anaph_food
  (next-node q5_anaph_food)
  =>
  (ask-question "Симптомы начались в течение 30 минут после употребления арахиса/орехов/моллюсков?" leaf_anaph_food leaf_anaph_unknown))

(defrule leaf_anaph_food
  (next-node leaf_anaph_food)
  =>
  (printout t "Диагноз: Анафилаксия пищевая (тяжёлая IgE-опосредованная)" crlf)
  (halt))

(defrule leaf_anaph_unknown
  (next-node leaf_anaph_unknown)
  =>
  (printout t "Диагноз: Анафилаксия неясной этиологии" crlf)
  (halt))

(defrule q3_resp
  (next-node q3_resp)
  =>
  (ask-question "Реакция связана с дыхательной системой?" q4_season q4_skin))

(defrule q4_season
  (next-node q4_season)
  =>
  (ask-question "Симптомы (чихание, зуд в носу, ринорея) появляются сезонно во время цветения?" leaf_seasonal q5_perennial))

(defrule leaf_seasonal
  (next-node leaf_seasonal)
  =>
  (printout t "Диагноз: Сезонный аллергический ринит (поллиноз)" crlf)
  (halt))

(defrule q5_perennial
  (next-node q5_perennial)
  =>
  (ask-question "Симптомы сохраняются круглый год и усиливаются дома или на рабочем месте?" leaf_perennial q6_asthma))

(defrule leaf_perennial
  (next-node leaf_perennial)
  =>
  (printout t "Диагноз: Круглогодичный аллергический ринит (на бытовые аллергены)" crlf)
  (halt))

(defrule q6_asthma
  (next-node q6_asthma)
  =>
  (ask-question "Основной симптом — приступы удушья, свистящее дыхание?" leaf_asthma leaf_nonallergic))

(defrule leaf_asthma
  (next-node leaf_asthma)
  =>
  (printout t "Диагноз: Аллергическая бронхиальная астма" crlf)
  (halt))

(defrule leaf_nonallergic
  (next-node leaf_nonallergic)
  =>
  (printout t "Диагноз: Неаллергический ринит" crlf)
  (halt))

(defrule q4_skin
  (next-node q4_skin)
  =>
  (ask-question "Реакция проявляется на коже?" q5_contact_imm q5_gut))

(defrule q5_contact_imm
  (next-node q5_contact_imm)
  =>
  (ask-question "При контакте с аллергеном возникают зуд, волдыри, покраснение?" leaf_contact_urticaria q6_delayed))

(defrule leaf_contact_urticaria
  (next-node leaf_contact_urticaria)
  =>
  (printout t "Диагноз: Контактная крапивница (IgE-опосредованная)" crlf)
  (halt))

(defrule q6_delayed
  (next-node q6_delayed)
  =>
  (ask-question "Сыпь появилась через 12–48 часов после контакта с металлом/косметикой, похожа на экзему?" leaf_delayed_contact q7_wheals))

(defrule leaf_delayed_contact
  (next-node leaf_delayed_contact)
  =>
  (printout t "Диагноз: Аллергический контактный дерматит (IV тип, замедленный)" crlf)
  (halt))

(defrule q7_wheals
  (next-node q7_wheals)
  =>
  (ask-question "Беспокоят спонтанно возникающие зудящие волдыри («крапивница»), длятся менее 24 часов?" leaf_chronic_urticaria leaf_atopic))

(defrule leaf_chronic_urticaria
  (next-node leaf_chronic_urticaria)
  =>
  (printout t "Диагноз: Хроническая спонтанная крапивница" crlf)
  (halt))

(defrule leaf_atopic
  (next-node leaf_atopic)
  =>
  (printout t "Диагноз: Атопический дерматит (экзема)" crlf)
  (halt))

(defrule q5_gut
  (next-node q5_gut)
  =>
  (ask-question "Реакция связана с желудочно-кишечным трактом (ЖКТ)?" q6_gut_child q6_exercise))

(defrule q6_gut_child
  (next-node q6_gut_child)
  =>
  (ask-question "Симптомы (боль в животе, тошнота, рвота, диарея) возникают у ребёнка до 2 лет при употреблении коровьего молока/яиц/сои?" leaf_food_milk q7_gut_other))

(defrule leaf_food_milk
  (next-node leaf_food_milk)
  =>
  (printout t "Диагноз: Пищевая аллергия (IgE-опосредованная, молоко/яйцо)" crlf)
  (halt))

(defrule q7_gut_other
  (next-node q7_gut_other)
  =>
  (ask-question "Симптомы ЖКТ появляются через несколько часов после употребления пшеницы/морепродуктов?" leaf_food_grain leaf_oral_syndrome))

(defrule leaf_food_grain
  (next-node leaf_food_grain)
  =>
  (printout t "Диагноз: Пищевая аллергия (IgE-опосредованная, злаки/рыба)" crlf)
  (halt))

(defrule leaf_oral_syndrome
  (next-node leaf_oral_syndrome)
  =>
  (printout t "Диагноз: Синдром оральной аллергии (на свежие фрукты/овощи)" crlf)
  (halt))

(defrule q6_exercise
  (next-node q6_exercise)
  =>
  (ask-question "Реакция возникает при физической нагрузке?" q7_exercise_food leaf_undefined))

(defrule q7_exercise_food
  (next-node q7_exercise_food)
  =>
  (ask-question "Симптомы появляются только после еды перед физической нагрузкой?" leaf_cholinergic leaf_exercise_no_food))

(defrule leaf_cholinergic
  (next-node leaf_cholinergic)
  =>
  (printout t "Диагноз: Холинергическая крапивница/анафилаксия, вызванная физической нагрузкой" crlf)
  (halt))

(defrule leaf_exercise_no_food
  (next-node leaf_exercise_no_food)
  =>
  (printout t "Диагноз: Крапивница от физической нагрузки (не связана с едой)" crlf)
  (halt))

(defrule leaf_undefined
  (next-node leaf_undefined)
  =>
  (printout t "Диагноз: Неопределённый тип (требуется обследование у аллерголога)" crlf)
  (halt))

(defrule q2_no
  (next-node q2_no)
  =>
  (ask-question "Реакция возникает не менее чем через 1 день (более 12–24 часов) после контакта с аллергеном?" q3_skin_delayed q3_physical))

(defrule q3_skin_delayed
  (next-node q3_skin_delayed)
  =>
  (ask-question "Реакция связана с кожей (сыпь, зуд)?" q4_local_delayed q4_fever_bite))

(defrule q4_local_delayed
  (next-node q4_local_delayed)
  =>
  (ask-question "Сыпь появилась в месте нанесения мази/пластыря или контакта с одеждой с металлическими деталями?" leaf_contact_delayed q5_drug_rash))

(defrule leaf_contact_delayed
  (next-node leaf_contact_delayed)
  =>
  (printout t "Диагноз: Аллергический контактный дерматит (IV тип, замедленный)" crlf)
  (halt))

(defrule q5_drug_rash
  (next-node q5_drug_rash)
  =>
  (ask-question "Сыпь генерализованная, появилась через несколько дней после приёма внутрь антибиотиков?" leaf_drug_rash leaf_nonallergic_urticaria))

(defrule leaf_drug_rash
  (next-node leaf_drug_rash)
  =>
  (printout t "Диагноз: Лекарственная сыпь (IV тип, замедленная)" crlf)
  (halt))

(defrule leaf_nonallergic_urticaria
  (next-node leaf_nonallergic_urticaria)
  =>
  (printout t "Диагноз: Неаллергическая крапивница" crlf)
  (halt))

(defrule q4_fever_bite
  (next-node q4_fever_bite)
  =>
  (ask-question "Реакция проявилась в виде лихорадки, болей в суставах после укуса насекомого?" leaf_systemic_delayed leaf_undefined_delayed))

(defrule leaf_systemic_delayed
  (next-node leaf_systemic_delayed)
  =>
  (printout t "Диагноз: Системная реакция замедленного типа на укус насекомого" crlf)
  (halt))

(defrule leaf_undefined_delayed
  (next-node leaf_undefined_delayed)
  =>
  (printout t "Диагноз: Неопределённый тип" crlf)
  (halt))

(defrule q3_physical
  (next-node q3_physical)
  =>
  (ask-question "Реакция всегда возникает на холод/тепло/солнце?" q4_physical_factor leaf_undefined_physical))

(defrule q4_physical_factor
  (next-node q4_physical_factor)
  =>
  (ask-question "Симптомы (волдыри, зуд, покраснение) появляются под воздействием провоцирующего фактора?" leaf_physical_urticaria leaf_idiopathic))

(defrule leaf_physical_urticaria
  (next-node leaf_physical_urticaria)
  =>
  (printout t "Диагноз: Физическая крапивница (холодовая/солнечная/вибрационная)" crlf)
  (halt))

(defrule leaf_idiopathic
  (next-node leaf_idiopathic)
  =>
  (printout t "Диагноз: Идиопатическая крапивница" crlf)
  (halt))

(defrule leaf_undefined_physical
  (next-node leaf_undefined_physical)
  =>
  (printout t "Диагноз: Неопределённый тип" crlf)
  (halt))